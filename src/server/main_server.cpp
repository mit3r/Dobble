#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>
#include <optional>
#include <cstring>
#include <variant>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>
#include "../protocol/utils/SendAndReceiveUtils.hpp"
#include "../protocol/CommandFactory.hpp"
#include "../protocol/lobby/lobbyclient/SocketCommands.hpp"
using json = nlohmann::json;
struct Client {
    int socket;
    std::string username;
    bool logged_in;
};

struct ServerState {
    std::vector<Client*> clients;
    std::mutex clients_mutex; 
};

ServerState g_server;

struct ServerCommandVisitor {
    int client_sock;        
    CommandFactory& factory;
    
    ServerCommandVisitor(int sock, CommandFactory& f) : client_sock(sock), factory(f) {}

    void operator()(const SenderPingCommand& cmd) {
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

    void operator()(const SenderJoinGameCommand& cmd) {
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

    void operator()(const SenderGetLobbyInfoCommand& cmd) {
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
    
    template <typename T>
    void operator()(const T& cmd) {
        if constexpr (std::is_base_of_v<LobbyServerMessageCore, T>) {
             std::cout << "[INFO] Otrzymano inna komende: " << cmd.command << std::endl;
        } else {
             std::cout << "[INFO] Otrzymano nieznany typ wariantu." << std::endl;
        }
    }

    void operator()(const std::monostate&) {
        std::cerr << "[ERROR] Nie udało się rozpoznać komendy w JSON." << std::endl;
    }
};

void client_handler(int client_sock) {
    Client* me = new Client{client_sock, "Unknown", false};
    {
        std::lock_guard<std::mutex> lock(g_server.clients_mutex);
        g_server.clients.push_back(me);
    }

    std::cout << "[SERVER] New client connected: " << client_sock << std::endl;

    CommandFactory command_factory;

    while (true) {
        std::optional<json> msg_opt = receive_json_packet(client_sock);

        if (!msg_opt.has_value()) {
            break;         } // hmmmm

        json msg = msg_opt.value();
        if (msg.contains("command")) {
            std::string cmd_name = msg["command"];
            AnyCommand command_variant = command_factory.get(cmd_name, msg);
            ServerCommandVisitor visitor(client_sock, command_factory);
            std::visit(visitor, command_variant);
        } else {
            std::cerr << "[WARNING] Otrzymano JSON bez pola 'command'" << std::endl;
        }
    }

    std::cout << "[SERVER] Client disconnected: " << client_sock << std::endl;
    close(client_sock);

    {
        std::lock_guard<std::mutex> lock(g_server.clients_mutex);
        auto it = std::remove(g_server.clients.begin(), g_server.clients.end(), me);
        g_server.clients.erase(it, g_server.clients.end());
    }
    delete me;
}

int main(int argc, char *argv[]) {
    int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(1500);
    socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // INADDR_ANY 

    if (bind(fd, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) < 0) {
        perror("bind"); return 1;
    }

    if (listen(fd, 10) < 0) {
        perror("listen"); return 1;
    }

    std::cout << "Serwer nasłuchuje na porcie 1500..." << std::endl;

    while (true) {
        int client_sock = accept(fd, NULL, NULL);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }
        std::thread t(client_handler, client_sock);
        t.detach();
    }

    close(fd);
    return 0;
}