#include "LobbyClientHandler.hpp"
#include "ServerUtils.hpp"
#include "ServerCommandVisitor.hpp"
#include "../../protocol/utils/SendAndReceiveUtils.hpp"
#include "../../protocol/CommandFactory.hpp"
#include "../../protocol/lobby/lobbyclient/SocketCommands.hpp"
#include <iostream>
#include <optional>
#include <unistd.h>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

extern LobbyServerState g_lobby_server;

void client_handler(int client_sock)
{
    std::shared_ptr<Client> me = std::make_shared<Client>();
    me->socket = client_sock;
    me->logged_in = false;

    g_lobby_server.addClient(me);
    std::cout << "[SERVER] New client connected: " << client_sock << std::endl;

    CommandFactory command_factory;

    while (true)
    {
        std::optional<json> msg_opt = receive_json_packet(client_sock);

        if (!msg_opt.has_value())
        {
            break;
        }

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

    g_lobby_server.removeClient(me);
}
