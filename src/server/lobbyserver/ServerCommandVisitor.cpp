#include "ServerCommandVisitor.hpp"

extern ServerState g_server;

ServerCommandVisitor::ServerCommandVisitor(int sock, CommandFactory& f) 
    : client_sock(sock), factory(f) {}

void ServerCommandVisitor::sendErrorResponse(
    const std::string& commandName, 
    const std::string& code, 
    const std::string& message) 
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

void ServerCommandVisitor::operator()(const SenderLoginCommand& cmd) {
    std::shared_ptr<Client> client = findClientBySocket(g_server, client_sock);

    if (!client){
        std::cout << "Brak clienta o socketcie" << client_sock << std::endl;
    }
    std::cout << client->logged_in << std::endl;
    if (client->logged_in == true) {
        sendErrorResponse(
            "login", 
            "400", 
            "You are already logged!"
        );
        return;
    }
    
    if (findClientByNickname(g_server, cmd.data_obj->nickname) != nullptr){
        sendErrorResponse(
            "login",
            "401",
            "Nickaname is already taken!"
        );
        return;
    }
    
    client->nickname = cmd.data_obj->nickname;
    client->logged_in = 1; 

    ResponseLoginCommand response;
    response.command = "login";
    response.lobby_server_id = "MainServer_v1"; 
    response.client_id = cmd.client_id;
    
    ResponseLoginCommand::data d;
    d.message = "OK";
    response.data_obj = d;
    json j = response;
    send_json_packet(client_sock, j);
}

void ServerCommandVisitor::operator()(const SenderPingCommand& cmd) {
    std::cout << "[CMD] Otrzymano PING. Message: " 
              << (cmd.data_obj ? cmd.data_obj->message : "brak") << std::endl;

    ResponsePingCommand response;
    response.command = "ping";
    response.lobby_server_id = "MainServer_v1"; 
    response.client_id = cmd.client_id;
    
    ResponsePingCommand::data d;
    d.message = "pong";
    response.data_obj = d;

    json j = response;
    send_json_packet(client_sock, j);
}

void ServerCommandVisitor::operator()(const SenderJoinGameCommand& cmd) {
    std::string game_id = cmd.data_obj ? cmd.data_obj->game_id : "unknown";
    std::cout << "[CMD] Klient chce dolaczyc do gry: " << game_id << std::endl;

    ResponseJoinGameCommand response;
    response.command = "join_game";
    response.lobby_server_id = "MainServer_v1";
    response.client_id = cmd.client_id;
    ResponseJoinGameCommand::data d;
    d.status = "OK";
    d.role = "PLAYER";
    d.game_info.game_id = game_id;
    d.game_info.name = "Super Gra";
    response.data_obj = d;
    json j = response;
    send_json_packet(client_sock, j);
}

void ServerCommandVisitor::operator()(const SenderGetLobbyInfoCommand& cmd) {
    std::cout << "[CMD] Pobieranie listy gier..." << std::endl;
    ResponseGetLobbyInfoCommand response;
    response.command = "getinfolobby";
    response.lobby_server_id = "MainServer_v1";
    ResponseGetLobbyInfoCommand::data d;
    d.page = "1";
    GameStruct g1;
    g1.game_id = "game_1";
    g1.game_name = "Pokoj #1";
    g1.players = "2";
    g1.max_players = "4";
    g1.status = "WAITING";
    d.actual_games.push_back(g1);

    response.data_obj = d;

    json j = response;
    send_json_packet(client_sock, j);
}

void ServerCommandVisitor::operator()(const std::monostate&) {
    std::cerr << "[ERROR] Nie udało się rozpoznać komendy w JSON." << std::endl;
}
