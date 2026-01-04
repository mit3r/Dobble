    #include "ServerCommandVisitor.hpp"
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

extern LobbyServerState g_lobby_server;

ServerCommandVisitor::ServerCommandVisitor(int sock, LobbyCommandFactory &f)
    : client_sock(sock), factory(f) {}

ServerCommandVisitor::ServerCommandVisitor(int sock, ServersCommandFactory &f)
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

void ServerCommandVisitor::operator()(const SenderLoginCommand &cmd)
{
    std::shared_ptr<Client> client = g_lobby_server.findClientBySocket(client_sock);
    if (client->logged_in == true)
    {
        sendErrorResponse(
            "login",
            "400",
            "You are already logged!");
        return;
    }

    if (g_lobby_server.findClientByNickname(cmd.data_obj->nickname) != nullptr)
    {
        sendErrorResponse(
            "login",
            "401",
            "Nickaname is already taken!");
        return;
    }

    client->nickname = cmd.data_obj->nickname;
    client->logged_in = 1;

    ResponseLoginCommand response;
    response.command = "login";
    response.lobby_server_id = "MainServer_v1";
    response.client_id = gen_random(16);

    ResponseLoginCommand::data d;
    d.message = "OK";
    response.data_obj = d;
    json j = response;
    send_json_packet(client_sock, j);
}

void ServerCommandVisitor::operator()(const SenderPingCommand &cmd)
{
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

void ServerCommandVisitor::operator()(const SenderGetLobbyInfoCommand &cmd)
{
    std::cout << "[CMD] Pobieranie listy gier..." << std::endl;
    std::string page = cmd.data_obj->page;
    int page_num = std::stoi(page);
    if (page_num < 1) page_num = 1;
    ResponseGetLobbyInfoCommand response;
    response.command = "getinfolobby";
    response.lobby_server_id = "MainServer_v1";
    ResponseGetLobbyInfoCommand::data d;
    d.page = page;
    std::vector<std::shared_ptr<GameServer>> actual_games = g_uds_server->getAllGameServers();
    

    for (size_t i = static_cast<size_t>((page_num - 1) * 12); i < static_cast<size_t>(page_num * 12) && i < actual_games.size(); ++i)
    {
        GameStruct gs;
        gs.game_id = actual_games[i]->server_id;
        gs.game_name = actual_games[i]->servername;
        gs.max_players = std::to_string(actual_games[i]->max_players);
        gs.players = std::to_string(actual_games[i]->players);
        gs.nicknames = {};
        gs.ip = actual_games[i]->ip;
        gs.port = actual_games[i]->port;
        gs.status = actual_games[i] ->status;
        std::cout << "[GAME] ID: " << gs.game_id
                  << ", Name: " << gs.game_name
                  << ", Players: " << gs.players << "/" << gs.max_players
                  << ", Status: " << gs.status
                  << ", IP: " << gs.ip
                  << ", Port: " << gs.port
                  << std::endl;
        d.actual_games.emplace_back(std::move(gs));
    }
    d.next_page = (actual_games.size() > static_cast<size_t>(page_num * 12)) ? std::optional<std::string>(std::to_string(page_num + 1)) : std::nullopt;

    response.data_obj = d;

    json j = response;
    send_json_packet(client_sock, j);
}

void ServerCommandVisitor::operator()(const SenderCreateLobbyCommand &cmd)
{
    std::cout << "[CMD] Tworzenie nowego lobby: " << cmd.data_obj->game_name << std::endl;

    std::shared_ptr<GameServer> game = std::make_shared<GameServer>();
    game->server_id = gen_random(16);
    game->servername = cmd.data_obj->game_name;
    game->registered = false;
    game->socket = -1;
    int dynamic_port = find_available_port();
    game->port = dynamic_port;
    game->ip = "0.0.0.0";


    g_uds_server->addGameServer(game);

    const std::string &lobby_uds_path = g_lobby_server.getUDSPath();

    pid_t pid = fork();
    if (pid == 0)
    {
        std::string server_id_arg = game->server_id;
        std::string port_arg_str = std::to_string(dynamic_port);
        
        execl("./dobble_gameserver",
              "dobble_gameserver",
              server_id_arg.c_str(),
              port_arg_str.c_str(),
              lobby_uds_path.c_str(),
              (char *)NULL);
        perror("execl");
        exit(1);
    }
    else if (pid > 0)
    {
        game->process_pid = pid;  
        game->port = dynamic_port; 
        game->status = GameEnums::toString(GameEnums::GameStatus::INIT);
        std::cout << "[INFO] Uruchomiono game server z PID: " << pid
                  << ", ID: " << game->server_id
                  << ", Port: " << dynamic_port
                  << std::endl;

        ResponseCreateLobbyCommand response;
        response.command = "create_lobby";
        response.lobby_server_id = "MainServer_v1";
        response.client_id = cmd.client_id;
        response.game_id = game->server_id;

        ResponseCreateLobbyCommand::data d;
        d.message = "OK";
        d.ip = "0.0.0.0";
        d.port = std::to_string(dynamic_port);
        response.data_obj = d;

        json j = response;
        send_json_packet(client_sock, j);
    }
    else
    {
        perror("fork");
        sendErrorResponse(
            "create_lobby",
            "500",
            "Nie można utworzyć game servera");
    }
}

void ServerCommandVisitor::operator()(const SenderRegisterGameServerCommand &cmd)
{
    std::cout << "[CMD] Game server rejestruje się: ";
    if (cmd.data_obj) {
        std::cout << "IP: " << cmd.data_obj->ip 
                  << ", Port: " << cmd.data_obj->port
                  << ", Game: " << cmd.data_obj->game_name
                  << ", Game_id: " << cmd.game_id.value_or("brak") << std::endl;
    }


    if (cmd.game_id.has_value()) {
        std::string game_id = cmd.game_id.value();
        auto game = g_uds_server->getGameServerById(game_id);
        
        if (game && cmd.data_obj) {
            game->registered = true;
            game->socket = client_sock;
            game->ip = cmd.data_obj->ip;
            game->port = std::stoi(cmd.data_obj->port);
            game->max_players = std::stoi(cmd.data_obj->max_players);
            game ->server_socket = client_sock;
            
            std::cout << "[UDS] Game server zarejestrowany pomyślnie: " << game_id << std::endl;
            
            ResponseRegisterGameServerCommand response;
            response.command = "register_game_server";
            response.lobby_server_id = "MainServer_v1";
            response.game_id = game_id;
            
            ResponseRegisterGameServerCommand::data d;
            d.message = "Registration successful";
            response.data_obj = d;
            
            json j = response;
            send_json_packet(client_sock, j);
        } else {
            std::cerr << "[ERROR] Nie znaleziono game servera o ID: " 
                      << (cmd.game_id.value_or("brak")) << std::endl;
            
            sendErrorResponse(
                "register_game_server",
                "404",
                "Game server not found");
        }
    } else {
        std::cerr << "[ERROR] Brak game_id w komendzie rejestracji" << std::endl;
    }
}


void ServerCommandVisitor::operator()(const ResponseRegisterGameServerCommand &cmd)
{
    std::cout << "[CMD] Otrzymano odpowiedź rejestracji game servera: "
              << (cmd.data_obj ? cmd.data_obj->message : "brak") << std::endl;

}

void ServerCommandVisitor::operator()(const SenderEndGameCommand &cmd)
{
    std::cout << "[CMD] Game server kończy działanie: " << cmd.game_id.value_or("brak") << std::endl;
    
    if (cmd.game_id.has_value()) {
        std::string game_id = cmd.game_id.value();
        
        g_uds_server->removeGameServer(client_sock);

        std::cout << "[UDS] Game server " << game_id << " usunięty z listy" << std::endl;
    } else {
        std::cerr << "[ERROR] No game_id in command" << std::endl;
    }
}

void ServerCommandVisitor::operator()(const SenderGameServerStatusCommand &cmd)
{
    std::cout << "[CMD] Game server status update: " << cmd.game_id.value_or("brak") << std::endl;
    
    if (cmd.game_id.has_value() && cmd.data_obj) {
        std::string game_id = cmd.game_id.value();
        auto game = g_uds_server->getGameServerBySocket(client_sock);
        
        if (game) {
            game->players = std::stoi(cmd.data_obj->player_count);
            game->max_players = std::stoi(cmd.data_obj->max_players);
            game->status = cmd.data_obj->status;
            
            std::cout << "[UDS] Updated game server " << game_id 
                      << " - Players: " << game->players 
                      << "/" << game->max_players 
                      << ", Status: " << cmd.data_obj->status << std::endl;
            
            ResponseGameServerStatusCommand response;
            response.command = "game_server_status";
            response.lobby_server_id = "MainServer_v1";
            response.game_id = game_id;
            
            ResponseGameServerStatusCommand::data d;
            d.message = "Status updated successfully";
            response.data_obj = d;
            
            json j = response;
            send_json_packet(client_sock, j);
        } else {
            std::cerr << "[ERROR] Game server not found by socket: " << client_sock << std::endl;
        }
    } else {
        std::cerr << "[ERROR] Missing game_id or data in status command" << std::endl;
    }
}


void ServerCommandVisitor::operator()(const std::monostate &)
{
    std::cerr << "[ERROR] Nie udało się rozpoznać komendy w JSON." << std::endl;
};
