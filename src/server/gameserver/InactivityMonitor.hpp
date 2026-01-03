#ifndef INACTIVITY_MONITOR_HPP
#define INACTIVITY_MONITOR_HPP

#include "GameStateManager.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <unistd.h>

extern GameStateManager g_game_server;
extern bool g_server_running;

void inactivity_monitor_thread();

#endif
