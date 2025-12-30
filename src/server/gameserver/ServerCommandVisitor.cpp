#include "ServerCommandVisitor.hpp"
#include "GameStateManager.hpp"
#include "GameLogic.hpp"

std::string gen_random(const int len)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

extern GameStateManager g_game_server;
extern GameLogic* g_game_logic;

ServerCommandVisitor::ServerCommandVisitor(int sock, ServersCommandFactory &f)
    : client_sock(sock), factory(f) {}

ServerCommandVisitor::ServerCommandVisitor(int sock, GameCommandFactory &f)
    : client_sock(sock), factory(f) {}

void ServerCommandVisitor::sendErrorResponse(
    const std::string &commandName,
    const std::string &code,
    const std::string &message)
{
    ErrorResponse e;
    e.command = commandName;
    e.lobby_server_id = "MainServer_v1";

    ErrorStruct err;
    err.code = std::stoi(code);
    err.message = message;
    e.error = err;

    json j = e;

    send_json_packet(client_sock, j);
}

void ServerCommandVisitor::operator()(const SenderJoinGameCommand &cmd){
    std::optional<std::string> client_id_opt = cmd.client_id;
    std::cout << "[CMD] Klient chce dolaczyc do gry: " << (cmd.data_obj ? cmd.data_obj->game_id : "brak") << std::endl;

    if (!client_id_opt.has_value() || !cmd.data_obj) {
        std::cout << "[ERROR] Invalid join command" << std::endl;
        sendErrorResponse("join_game", "400", "Invalid request");
        return;
    }

    std::string client_id = cmd.client_id.value();
    std::string nickname = cmd.client_nickname;
    
    std::cout << "[INFO] Client '" << client_id_opt.value() << "' (ID: " << client_id << ") joining..." << std::endl;
    
    if (g_game_server.isGameFull()) {
        std::cout << "[INFO] Game is full" << std::endl;
        ResponseJoinGameCommand response;
        response.command = "join_game";
        response.data_obj = ResponseJoinGameCommand::data{};
        response.data_obj->status = "GAME_FULL";
        response.data_obj->role = "OBSERVER";
        response.data_obj->game_info = ShortGameInfo{};
        
        json j = response;
        send_json_packet(client_sock, j);
        return;
    }
    
    g_game_server.addPlayer(client_id, nickname);
    std::cout << "[INFO] Player " << client_id << " (" << nickname << ") joined. "
              << "Players: " << g_game_server.getPlayerCount() << "/" << g_game_server.getMaxPlayers() << std::endl;
    
    ResponseJoinGameCommand response;
    response.command = "join_game";
    response.data_obj = ResponseJoinGameCommand::data{};
    response.data_obj->status = "JOINED";
    response.data_obj->role = cmd.data_obj->role;
    
    response.data_obj->game_info.game_id = cmd.data_obj->game_id;
    response.data_obj->game_info.name = g_game_server.getGameName();
    
    json j = response;
    send_json_packet(client_sock, j);
    
    g_game_server.setGameStatus("waiting");

    std::cout << "[INFO] Player joined. Status: " << g_game_server.getGameStatus() 
              << ", Players: " << g_game_server.getPlayerCount() << "/" << g_game_server.getMaxPlayers() << std::endl;
}

void ServerCommandVisitor::operator()(const SenderStartGameCommand &cmd){
    std::cout << "[CMD] Client requests to start game" << std::endl;
    
    ResponseStartGameCommand response;
    response.command = "start_game";
    response.data_obj = ResponseStartGameCommand::data{};
    
    if (g_game_server.getGameStatus() != "waiting") {
        response.data_obj->success = false;
        response.data_obj->message = "Game already started or finished";
        response.data_obj->status = g_game_server.getGameStatus();
        
        json j = response;
        send_json_packet(client_sock, j);
        return;
    }
    
    if (!g_game_server.isGameReady()) {
        response.data_obj->success = false;
        response.data_obj->message = "Not enough players (need at least 2)";
        response.data_obj->status = "waiting";
        
        json j = response;
        send_json_packet(client_sock, j);
        return;
    }
    
    if (g_game_logic) {
        std::cout << "[INFO] Starting game manually!" << std::endl;
        g_game_logic->InitializeGame();
        
        response.data_obj->success = true;
        response.data_obj->message = "Game started successfully";
        response.data_obj->status = "GAME_ACTIVE";
    } else {
        response.data_obj->success = false;
        response.data_obj->message = "Game logic not initialized";
        response.data_obj->status = "error";
    }
    
    json j = response;
    send_json_packet(client_sock, j);
}

void ServerCommandVisitor::operator()(const SenderSendGameInfoCommand &cmd){
    std::cout << "[CMD] Client requests game info" << std::endl;
    
    if (!g_game_logic) {
        sendErrorResponse("send_game_info", "500", "Game logic not initialized");
        return;
    }
    
    ResponseSendGameInfoCommand response;
    response.command = "send_game_info";
    
    response.data_obj = ResponseSendGameInfoCommand::data{};
    response.data_obj->game_id = cmd.data_obj ? cmd.data_obj->game_id : "";
    
    auto& turn = g_game_server.getCurrentTurn();
    
    response.data_obj->actual_turn.turn_id = std::to_string(turn.turn_id);
    response.data_obj->actual_turn.active = turn.is_active;
    response.data_obj->actual_turn.winner_id = turn.winner_id;
    response.data_obj->actual_turn.status = g_game_server.getGameStatus();
    
    response.data_obj->actual_turn.images_on_table = turn.images_on_table;
    
    if (!turn.images_on_table.empty()) {
        response.data_obj->actual_turn.topCardId = turn.images_on_table.front();
    } else {
        response.data_obj->actual_turn.topCardId = -1;
    }
    
    response.data_obj->actual_turn.cardsLeft = 57 - turn.turn_id;
    
    response.data_obj->actual_turn.clients_data = g_game_server.getPlayersGameInfo();
    
    response.data_obj->actual_turn.past_turns = turn.past_turns;
    
    json j = response;
    send_json_packet(client_sock, j);
};

void ServerCommandVisitor::operator()(const SenderMatchSymbolCommand &cmd) {
    std::cout << "[CMD] Client tries to match symbol" << std::endl;
    
    if (!g_game_logic || !cmd.client_id.has_value() || !cmd.data_obj) {
        sendErrorResponse("match_symbol", "400", "Invalid request");
        return;
    }
    
    std::string client_id = cmd.client_id.value();
    int turn_id = cmd.data_obj->turn_id;
    int symbol_id = cmd.data_obj->symbol_id;
    
    std::cout << "[INFO] Client " << client_id << " claims symbol " << symbol_id 
              << " for turn " << turn_id << std::endl;

    auto result = g_game_logic->ProcessMatch(client_id, turn_id, symbol_id);
    
    ResponseMatchSymbolCommand response;
    response.command = "match_symbol";
    response.data_obj = ResponseMatchSymbolCommand::data{};
    response.data_obj->success = result.success;
    response.data_obj->message = result.message;
    response.data_obj->points_awarded = result.points_awarded;
    
    if (g_game_server.hasPlayer(client_id)) {
        response.data_obj->new_score = g_game_server.getPlayer(client_id).score;
    } else {
        response.data_obj->new_score = 0;
    }
    
    json j = response;
    send_json_packet(client_sock, j);
    
    if (result.success && !g_game_logic->IsGameOver()) {
        std::cout << "[INFO] Correct match! Moving to next round..." << std::endl;
        g_game_logic->NextRound();
    }
    
    if (g_game_logic->IsGameOver()) {
        std::cout << "[INFO] Game Over!" << std::endl;
    }
};

void ServerCommandVisitor::operator()(const ResponseSendGameInfoCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseStartGameCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseMatchSymbolCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseGameClientPingCommand &cmd){};
void ServerCommandVisitor::operator()(const SenderGameClientPingCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseJoinGameCommand &cmd){};
void ServerCommandVisitor::operator()(const SenderLeaveRoomCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseLeaveRoomCommand &cmd){};

void ServerCommandVisitor::operator()(const std::monostate &)
{
    std::cerr << "[ERROR] Nie udało się rozpoznać komendy w JSON." << std::endl;
};
