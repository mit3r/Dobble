#include "ServerCommandVisitor.hpp"
#include "GameStateManager.hpp"
#include "GameLogic.hpp"
#include "GameStatusManager.hpp"
#include <protocol/GameEnums.hpp>
#include <thread>
#include <chrono>

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
    std::cout << "[CMD] Client wants to join game: " << (cmd.data_obj ? cmd.data_obj->game_id : "none") << std::endl;
    std::cout << "[DEBUG] client_nickname field: '" << cmd.client_nickname << "'" << std::endl;

    if (!client_id_opt.has_value() || !cmd.data_obj) {
        std::cout << "[ERROR] Invalid join command" << std::endl;
        sendErrorResponse("join_game", "400", "Invalid request");
        return;
    }
    if (g_game_server.isGameOver()) {
        std::cout << "[ERROR] Game is already over" << std::endl;
        sendErrorResponse("join_game", "400", "Game is already over");
        return;
    }

    std::string client_id = cmd.client_id.value();
    std::string nickname = cmd.client_nickname;
    
    std::cout << "[INFO] Client '" << client_id_opt.value() << "' (ID: " << client_id << ") joining with nickname: '" << nickname << "'" << std::endl;
    



    if (g_game_server.hasPlayer(client_id)) {
        std::cout << "[INFO] Player " << client_id << " already joined" << std::endl;
        ResponseJoinGameCommand response;
        response.command = "join_game";
        response.game_id = g_game_server.getGameId();

        response.data_obj = ResponseJoinGameCommand::data{};
        response.data_obj->status = "ALREADY_JOINED";
        response.data_obj->role = cmd.data_obj->role;
        
        json j = response;
        send_json_packet(client_sock, j);
        return;
    }
    
    if (g_game_server.isGameFull()) {
        std::cout << "[INFO] Game is full" << std::endl;
        ResponseJoinGameCommand response;
        response.command = "join_game";
        response.game_id = g_game_server.getGameId();
        response.data_obj = ResponseJoinGameCommand::data{};
        response.data_obj->status = "GAME_FULL";
        response.data_obj->role = "OBSERVER";
        
        json j = response;
        send_json_packet(client_sock, j);
        return;
    }


    
    g_game_server.addPlayer(client_id, nickname, client_sock);
    std::cout << "[INFO] Player " << client_id << " (" << nickname << ") joined. "
              << "Players: " << g_game_server.getPlayerCount() << "/" << g_game_server.getMaxPlayers() << std::endl;
    
    if (g_game_server.getGameStatus() == GameEnums::toString(GameEnums::GameStatus::GAME_ACTIVE)) {
        std::cout << "[ERROR] Game is already playing, cannot join" << std::endl;
        sendErrorResponse("join_game", "400", "Game is already playing");
        return;
    }
        g_game_server.setGameStatus(GameEnums::toString(GameEnums::GameStatus::WAITING));

    
    ResponseJoinGameCommand response;
    response.command = "join_game";
    response.game_id = g_game_server.getGameId();

    response.data_obj = ResponseJoinGameCommand::data{};
    response.data_obj->status = "JOINED";
    response.data_obj->role = "PLAYER";
    
    
    json j = response;
    send_json_packet(client_sock, j);
    if (g_game_server.getGameStatus() == GameEnums::toString(GameEnums::GameStatus::INIT)) {
        g_game_server.setGameStatus(GameEnums::toString(GameEnums::GameStatus::WAITING));
         }
    std::cout << "[INFO] Player joined. Status: " << g_game_server.getGameStatus() 
                    << ", Players: " << g_game_server.getPlayerCount() << "/" << g_game_server.getMaxPlayers() << std::endl;

    // Send status update to lobby server
    send_status_update_to_lobby();
}

void ServerCommandVisitor::operator()(const SenderStartGameCommand&){
    std::cout << "[CMD] Client requests to start game" << std::endl;
    
    ResponseStartGameCommand response;
    response.command = "start_game";
    response.game_id = g_game_server.getGameId();
    response.data_obj = ResponseStartGameCommand::data{};
    
    std::string current_status = g_game_server.getGameStatus();
    if (current_status != GameEnums::toString(GameEnums::GameStatus::WAITING) ) {
        response.data_obj->success = false;
        response.data_obj->message = "Game already started or finished";
        response.data_obj->status = current_status;
        
        json j = response;
        send_json_packet(client_sock, j);
        return;
    }
    
    if (!g_game_server.isGameReady()) {
        response.data_obj->success = false;
        response.data_obj->message = "Not enough players (need at least 2)";
        response.data_obj->status = GameEnums::toString(GameEnums::GameStatus::WAITING);
        
        json j = response;
        send_json_packet(client_sock, j);
        return;
    }
    
    if (g_game_logic) {
        std::cout << "[INFO] Starting game manually!" << std::endl;
        g_game_logic->InitializeGame();
        
        response.data_obj->success = true;
        response.data_obj->message = "Game started successfully";
        response.data_obj->status = GameEnums::toString(GameEnums::GameStatus::GAME_ACTIVE);
        
        // Send status update to lobby server
        send_status_update_to_lobby();
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
    response.client_id = cmd.client_id;
    response.game_id = g_game_server.getGameId();
    response.data_obj = ResponseSendGameInfoCommand::data{};
    response.data_obj->game_id = g_game_server.getGameInfo().game_id;
    
    const auto& turn = g_game_server.getCurrentTurn();
    
    response.data_obj->actual_turn.turn_id = std::to_string(turn.turn_id);
    response.data_obj->actual_turn.active = turn.is_active;
    response.data_obj->actual_turn.winner_nickname = turn.winner_nickname;
    response.data_obj->actual_turn.status = g_game_server.getGameStatus();
    
    response.data_obj->actual_turn.images_on_table = turn.images_on_table;
    
    if (!turn.images_on_table.empty()) {
        response.data_obj->actual_turn.topCardId = turn.images_on_table.front();
    } else {
        response.data_obj->actual_turn.topCardId = -1;
    }
    
    response.data_obj->actual_turn.cardsLeft = 57 - turn.turn_id - g_game_server.getPlayerCount() - 1;
    
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
    
    if (g_game_server.isGameOver()) {
        std::cout << "[ERROR] Game is already over, cannot match symbols" << std::endl;
        sendErrorResponse("match_symbol", "400", "Game is already over");
        return;
    }
    
    std::string client_id = cmd.client_id.value();
    int turn_id = cmd.data_obj->turn_id;
    int symbol_id = cmd.data_obj->symbol_id;
    
    std::cout << "[INFO] Client " << client_id << " claims symbol " << symbol_id 
              << " for turn " << turn_id << std::endl;

    auto result = g_game_logic->ProcessMatch(client_id, turn_id, symbol_id,
                                            g_game_server.getPlayer(client_id).nickname);
    
    ResponseMatchSymbolCommand response;
    response.command = "match_symbol";
    response.game_id = g_game_server.getGameId();
    response.data_obj = ResponseMatchSymbolCommand::data{};
    response.data_obj->success = result.success;
    response.data_obj->message = GameEnums::toString(result.result);
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

void ServerCommandVisitor::operator()(const ResponseSendGameInfoCommand &){};
void ServerCommandVisitor::operator()(const ResponseStartGameCommand &){};
void ServerCommandVisitor::operator()(const ResponseMatchSymbolCommand &){};
void ServerCommandVisitor::operator()(const ResponseGameClientPingCommand &){};
void ServerCommandVisitor::operator()(const SenderGameClientPingCommand &cmd){
    std::cout << "[CMD] Client ping received" << std::endl;
    
    if (!cmd.client_id.has_value()) {
        sendErrorResponse("ping", "400", "Invalid request: missing client_id");
        return;
    }
    
    std::string client_id = cmd.client_id.value();
    
    if (g_game_server.hasPlayer(client_id)) {
        g_game_server.updatePlayerPing(client_id);
        std::cout << "[PING] Updated ping timestamp for player: " << client_id << std::endl;
    } else { //xdxd
        auto client = g_game_server.findClientBySocket(client_sock);
        if (client) {
            g_game_server.updatePlayerPing(client->client_id);
            std::cout << "[PING] Updated ping timestamp for player: " << client->client_id << std::endl;
        }
    }
    
    ResponseGameClientPingCommand response;
    response.command = "ping";
    response.client_id = client_id;
    response.game_id = g_game_server.getGameId();
    response.data_obj = ResponseGameClientPingCommand::data{};
    response.data_obj->message = "pong";
    
    json j = response;
    send_json_packet(client_sock, j);
};
void ServerCommandVisitor::operator()(const ResponseJoinGameCommand &){};
void ServerCommandVisitor::operator()(const SenderLeaveRoomCommand &cmd){
    std::cout << "[CMD] Client requests to leave room" << std::endl;
    if (!g_game_server.hasPlayer(cmd.client_id.value_or(""))) {
        sendErrorResponse("leave_room", "400", "Client not in game");
        return;
    }

    std::string leaving_player = cmd.client_id.value();
    g_game_server.removePlayer(leaving_player);
    
    std::cout << "[INFO] Player " << leaving_player << " left. Remaining players: " 
              << g_game_server.getPlayerCount() << std::endl;

    ResponseLeaveRoomCommand response;
    response.command = "leave_room";
    response.game_id = g_game_server.getGameId();
    response.data_obj = ResponseLeaveRoomCommand::data{};
    response.data_obj->message = "LEFT";
    json j = response;
    send_json_packet(client_sock, j);
    send_status_update_to_lobby();
    close(client_sock);

    if (g_game_server.getPlayerCount() == 0 && g_game_server.isGameOver()) {

        exit(0);// xdd

    }


};
void ServerCommandVisitor::operator()(const ResponseLeaveRoomCommand &){};

void ServerCommandVisitor::operator()(const std::monostate &)
{
    std::cerr << "[ERROR] Failed to recognize command in JSON." << std::endl;
};
