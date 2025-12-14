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
#include "ServerCommandVisitor.hpp"
using json = nlohmann::json;
struct Client
{
    int socket;
    std::string nickname;
    bool logged_in;
};
struct GameServer{
    int socket;
    std::string server_id;
    bool registered;
};

struct ServerState
{
    std::vector<std::shared_ptr<Client>> clients;
    std::mutex clients_mutex;
};

std::shared_ptr<Client> findClientBySocket(ServerState &state, int targetSocket)
{
    std::lock_guard<std::mutex> lock(state.clients_mutex);

    auto it = std::find_if(state.clients.begin(), state.clients.end(),
                           [targetSocket](const std::shared_ptr<Client> &c)
                           {
                               return c->socket == targetSocket;
                           });

    if (it != state.clients.end())
    {
        return *it;
    }

    return nullptr;
}

std::shared_ptr<Client> findClientByNickname(ServerState &state, const std::string &targetNickname)
{
    std::lock_guard<std::mutex> lock(state.clients_mutex);

    auto it = std::find_if(state.clients.begin(), state.clients.end(),
                           [targetNickname](const std::shared_ptr<Client> &c)
                           {
                               return c->nickname == targetNickname;
                           });

    if (it != state.clients.end())
    {
        return *it;
    }

    return nullptr;
}

ServerState g_server;

void client_handler(int client_sock)
{
    // Client* me = new Client{client_sock, "Unknown", false};
    std::shared_ptr<Client> me = std::make_shared<Client>();
    me->socket = client_sock;
    me->logged_in = false;

    {
        std::lock_guard<std::mutex> lock(g_server.clients_mutex);
        g_server.clients.push_back(me);
    }
    std::cout << "[SERVER] New client connected: " << client_sock << std::endl;

    CommandFactory command_factory;

    while (true)
    {
        std::optional<json> msg_opt = receive_json_packet(client_sock);

        if (!msg_opt.has_value())
        {
            break;
        } // hmmmm

        json msg = msg_opt.value();
        if (msg.contains("command"))
        {
            std::string cmd_name = msg["command"];
            AnyCommand command_variant = command_factory.get(cmd_name, msg);

            ServerCommandVisitor visitor(client_sock, command_factory);
            std::visit(visitor, command_variant);
        }
        else
        {
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
}


void run_uds_server(const char* socket_path) {
    int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0)
    {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(1500);
    socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // INADDR_ANY

    if (bind(fd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (listen(fd, 10) < 0)
    {
        perror("listen");
        return 1;
    }

    std::cout << "Serwer nasłuchuje na porcie 1500..." << std::endl;

    while (true)
    {
        int client_sock = accept(fd, NULL, NULL);
        if (client_sock < 0)
        {
            perror("accept");
            continue;
        }
        std::thread t(client_handler, client_sock);
        t.detach();
    }

    close(fd);
    return 0;
}


int run_uds_client()
{
    int fd = socket(PF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("socket");
        return 1;
    }
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "/tmp/uds_socket", sizeof(server_addr.sun_path)
              - 1);
    
    if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        close(fd);
        return 1;
    }

    while (true)
    {

        // Tutaj można dodać logikę komunikacji z serwerem UDS

    }

}


int main(int argc, char *argv[])
{

    std::thread uds_thread(run_uds_server, "/tmp/uds_socket");
    uds_thread.join();

    std::thread server_thread(run_bsd_server);
    server_thread.join();



    return 0;

}   