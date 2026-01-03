#ifndef INACTIVITY_MONITOR_HPP
#define INACTIVITY_MONITOR_HPP

#include "GameStateManager.hpp"
#include "server/common/BaseClient.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>

extern GameStateManager g_game_server;
extern bool g_server_running;
extern UdsClient* g_uds_client;

void inactivity_monitor_thread();

#endif
