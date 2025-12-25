#include "server/common/ClientHandler.hpp"
#include "server/libraries.hpp"
#include "server/lobbyserver/LobbyServerState.hpp"
#include "server/lobbyserver/ServerCommandVisitor.hpp"
#include "server/network.hpp"
#include "protocol/LobbyCommandFactory.hpp"
#include "protocol/ServersCommandFactory.hpp"

#pragma once

void tcp_client_handler(int client_sock);
void uds_client_handler(int client_sock);
