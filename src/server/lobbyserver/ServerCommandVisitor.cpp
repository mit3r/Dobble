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

ServerCommandVisitor::ServerCommandVisitor(int sock, CommandFactory &f)
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

    if (!client)
    {
        std::cout << "Brak clienta o socketcie" << client_sock << std::endl;
    }
    std::cout << client->logged_in << std::endl;
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

void ServerCommandVisitor::operator()(const SenderJoinGameCommand &cmd)
{
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

    for (int i = page_num - 1; i < page_num + 10 && i < actual_games.size(); ++i)
    {
        GameStruct gs;
        gs.game_id = actual_games[i]->server_id;
        gs.game_name = actual_games[i]->servername;
        gs.max_players = std::to_string(actual_games[i]->max_players);
        gs.players = std::to_string(actual_games[i]->players);
        gs.status = actual_games[i]->registered ? "ACTIVE" : "INACTIVE";
        d.actual_games.push_back(gs);
    }

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
    g_uds_server->addGameServer(game);

    int dynamic_port = find_available_port();
    std::string port_arg = std::to_string(dynamic_port);

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

void ServerCommandVisitor::operator()(const std::monostate &)
{
    std::cerr << "[ERROR] Nie udało się rozpoznać komendy w JSON." << std::endl;
};
