#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include <server/ServerUtils.hpp>

class CommandFactory;


template<typename StateManager, typename VisitorCreator>
void handle_client(int client_sock, StateManager& server_state, VisitorCreator create_visitor);

#endif
