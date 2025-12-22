#include <server/ClientHandler.hpp>
#include "../protocol/CommandFactory.hpp"
#include "../protocol/lobby/lobbyclient/SocketCommands.hpp"
#include "../protocol/utils/SendAndReceiveUtils.hpp"
#include <iostream>
#include <optional>
#include <unistd.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

template<typename StateManager, typename VisitorCreator>
void handle_client(int client_sock, StateManager& server_state, VisitorCreator create_visitor)
{
    std::shared_ptr<Client> me = std::make_shared<Client>();
    me->socket = client_sock;
    me->logged_in = false;

    server_state.addClient(me);
    std::cout << "[SERVER] New client connected: " << client_sock << std::endl;

    CommandFactory command_factory;

    while (true)
    {
        std::optional<json> msg_opt = receive_json_packet(client_sock);

        if (!msg_opt.has_value())
        {
            break; //???
        }

        json msg = msg_opt.value();
        if (msg.contains("command"))
        {
            std::string cmd_name = msg["command"];
            AnyCommand command_variant = command_factory.get(cmd_name, msg);

            auto visitor = create_visitor(client_sock, command_factory);
            std::visit(visitor, command_variant);
        }
        else
        {
            std::cerr << "[WARNING] Otrzymano JSON bez pola 'command'" << std::endl;
        }
    }

    std::cout << "[SERVER] Client disconnected: " << client_sock << std::endl;
    close(client_sock);

    server_state.removeClient(me);
}

