#include "server/common/ClientHandler.hpp"
#include "server/libraries.hpp"
#include "server/lobbyserver/LobbyServerState.hpp"
#include "server/lobbyserver/ServerCommandVisitor.hpp"
#include "server/network.hpp"

#pragma once

void client_handler(int client_sock);
