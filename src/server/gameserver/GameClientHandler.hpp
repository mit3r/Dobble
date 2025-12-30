#include "server/common/ClientHandler.hpp"
#include "server/common/ServerUtils.hpp"
#include "server/gameserver/ServerCommandVisitor.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"
#include "server/gameserver/ServerRegistration.hpp"
#pragma once

void tcp_client_handler(int client_sock);
void uds_client_handler(int client_sock);
