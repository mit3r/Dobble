#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <optional>
#include <thread>
#include <mutex>
#include <chrono>
#include <protocol/json.hpp>
#include "protocol/lobby/room/SocketCommands.hpp"
#include "protocol/utils/SendAndReceiveUtils.hpp"

#pragma once

std::mutex g_cout_mutex;
bool g_running = true;
std::string g_client_id = "";
std::string g_game_id = "";

void receive_thread(int socket_fd) {
    while (g_running) {
        auto j_opt = receive_json_packet(socket_fd);
        if (!j_opt.has_value()) {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "[RECEIVE] Connection lost or error" << std::endl;
            g_running = false;
            break;
        }
        
        json j = j_opt.value();
        
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "[<<<] RECEIVED FROM SERVER:" << std::endl;
        std::cout << j.dump(2) << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        if (j.contains("command")) {
            std::string cmd = j["command"];
            
            if (cmd == "join_game") {
                if (j.contains("data") && j["data"].contains("status")) {
                    std::cout << "[INFO] Join status: " << j["data"]["status"] << std::endl;
                    if (j["data"].contains("game_info")) {
                        auto game_info = j["data"]["game_info"];
                        std::cout << "[INFO] Game ID: " << game_info.value("game_id", "?") << std::endl;
                        std::cout << "[INFO] Game Name: " << game_info.value("name", "?") << std::endl;
                    }
                }
            }
            else if (cmd == "send_game_info") {
                if (j.contains("data") && j["data"].contains("actual_turn")) {
                    auto turn = j["data"]["actual_turn"];
                    std::cout << "[GAME] Turn ID: " << turn.value("turn_id", "?") << std::endl;
                    std::cout << "[GAME] Active: " << (turn.value("active", false) ? "YES" : "NO") << std::endl;
                    std::cout << "[GAME] Top card: " << turn.value("topCardId", -1) << std::endl;
                    std::cout << "[GAME] Cards left: " << turn.value("cardsLeft", 0) << std::endl;
                    std::cout << "[GAME] Winner ID: " << turn.value("winner_id", "") << std::endl;
                    std::cout << "[GAME] Status: " << turn.value("status", "?") << std::endl;
                    
                    if (turn.contains("scoreboard")) {
                        std::cout << "[SCORE] Scoreboard: ";
                        for (auto& [id, score] : turn["scoreboard"].items()) {
                            std::cout << "Player" << id << "=" << score << " ";
                        }
                        std::cout << std::endl;
                    }
                    
                    if (turn.contains("images_on_table")) {
                        std::cout << "[CARDS] On table: ";
                        for (auto& card : turn["images_on_table"]) {
                            std::cout << card << " ";
                        }
                        std::cout << std::endl;
                    }
                    
                    if (turn.contains("clients_id_data")) {
                        std::cout << "[PLAYERS] Cards:" << std::endl;
                        for (auto& [id, cards] : turn["clients_id_data"].items()) {
                            std::cout << "  Player" << id << ": ";
                            for (auto& card : cards) {
                                std::cout << card << " ";
                            }
                            std::cout << std::endl;
                        }
                    }
                }
            }
            else if (cmd == "start_game") {
                if (j.contains("data")) {
                    auto data = j["data"];
                    std::cout << "[START] Success: " << (data.value("success", false) ? "YES" : "NO") << std::endl;
                    std::cout << "[START] Message: " << data.value("message", "?") << std::endl;
                    std::cout << "[START] Status: " << data.value("status", "?") << std::endl;
                }
            }
            else if (cmd == "match_symbol") {
                if (j.contains("data")) {
                    auto data = j["data"];
                    std::cout << "[MATCH] Result: " << data.value("message", "?") << std::endl;
                    std::cout << "[MATCH] Success: " << (data.value("success", false) ? "YES" : "NO") << std::endl;
                    std::cout << "[MATCH] Points awarded: " << data.value("points_awarded", 0) << std::endl;
                    std::cout << "[MATCH] New score: " << data.value("new_score", 0) << std::endl;
                }
            }
            else {
                std::cout << "[INFO] Unknown command: " << cmd << std::endl;
            }
        }
        else if (j.contains("error")) {
            std::cout << "[ERROR] Server error:" << std::endl;
            std::cout << "  Code: " << j["error"].value("code", 0) << std::endl;
            std::cout << "  Message: " << j["error"].value("message", "?") << std::endl;
        }
        
        std::cout << std::string(60, '=') << std::endl;
    }
}

void send_ping(int sock) {
    SenderGameClientPingCommand cmd;
    cmd.command = "ping";
    cmd.client_id = g_client_id;
    cmd.data_obj = SenderGameClientPingCommand::data{};
    cmd.data_obj->message = "ping from test client";
    
    json j = cmd;
    send_json_packet(sock, j);
    std::cout << "[>>>] Sent PING" << std::endl;
}

void send_join_game(int sock, const std::string& game_id, const std::string& role = "PLAYER") {
    SenderJoinGameCommand cmd;
    cmd.command = "join_game";
    cmd.client_id = g_client_id;
    cmd.client_nickname = "TestPlayer_" + g_client_id;  // Dodaj nickname!
    cmd.data_obj = SenderJoinGameCommand::data{};
    cmd.data_obj->game_id = game_id;
    cmd.data_obj->role = role;
    
    g_game_id = game_id;
    
    json j = cmd;
    send_json_packet(sock, j);
    std::cout << "[>>>] Sent JOIN_GAME to " << game_id << " with nickname: " << cmd.client_nickname << std::endl;
}

void send_get_game_info(int sock) {
    SenderSendGameInfoCommand cmd;
    cmd.command = "send_game_info";
    cmd.client_id = g_client_id;
    cmd.data_obj = SenderSendGameInfoCommand::data{};
    cmd.data_obj->game_id = g_game_id;
    
    json j = cmd;
    send_json_packet(sock, j);
    std::cout << "[>>>] Sent GET_GAME_INFO" << std::endl;
}

void send_start_game(int sock) {
    SenderStartGameCommand cmd;
    cmd.command = "start_game";
    cmd.client_id = g_client_id;
    cmd.data_obj = SenderStartGameCommand::data{};
    cmd.data_obj->game_id = g_game_id;
    
    json j = cmd;
    send_json_packet(sock, j);
    std::cout << "[>>>] Sent START_GAME" << std::endl;
}

void send_match_symbol(int sock, int turn_id, int symbol_id) {
    SenderMatchSymbolCommand cmd;
    cmd.command = "match_symbol";
    cmd.client_id = g_client_id;
    cmd.data_obj = SenderMatchSymbolCommand::data{};
    cmd.data_obj->game_id = g_game_id;
    cmd.data_obj->turn_id = turn_id;
    cmd.data_obj->symbol_id = symbol_id;
    
    json j = cmd;
    send_json_packet(sock, j);
    std::cout << "[>>>] Sent MATCH_SYMBOL: turn=" << turn_id 
              << ", symbol=" << symbol_id << std::endl;
}

void print_help() {
    std::cout << "\n=== GAME TEST CLIENT COMMANDS ===" << std::endl;
    std::cout << "  ping                       - Send ping to server" << std::endl;
    std::cout << "  join <game_id>             - Join game with ID" << std::endl;
    std::cout << "  start                      - Start the game (manual)" << std::endl;
    std::cout << "  info                       - Get current game info" << std::endl;
    std::cout << "  match <turn> <symbol>      - Match symbol on cards" << std::endl;
    std::cout << "  auto                       - Auto-refresh game info every 1s" << std::endl;
    std::cout << "  stop                       - Stop auto-refresh" << std::endl;
    std::cout << "  help                       - Show this help" << std::endl;
    std::cout << "  quit                       - Exit client" << std::endl;
    std::cout << "==================================\n" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> [client_id]" << std::endl;
        return 1;
    }
    
    std::string server_ip = argv[1];
    int server_port = std::stoi(argv[2]);
    
    if (argc >= 4) {
        g_client_id = argv[3];
    } else {
        g_client_id = "test_client_" + std::to_string(getpid());
    }
    
    std::cout << "[INFO] Game Test Client starting..." << std::endl;
    std::cout << "[INFO] Server: " << server_ip << ":" << server_port << std::endl;
    std::cout << "[INFO] Client ID: " << g_client_id << std::endl;
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[ERROR] Cannot create socket" << std::endl;
        return 1;
    }
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);
    
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[ERROR] Cannot connect to server" << std::endl;
        close(sock);
        return 1;
    }
    
    std::cout << "[INFO] Connected to game server!" << std::endl;
    print_help();
    
    std::thread receiver(receive_thread, sock);
    
    bool auto_refresh = false;
    std::thread auto_thread;
    
    std::string line;
    std::cout << ">>> ";
    while (g_running && std::getline(std::cin, line)) {
        if (line.empty()) {
            std::cout << ">>> ";
            continue;
        }
        
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        
        if (cmd == "quit" || cmd == "exit") {
            g_running = false;
            break;
        }
        else if (cmd == "help") {
            print_help();
        }
        else if (cmd == "ping") {
            send_ping(sock);
        }
        else if (cmd == "join") {
            std::string game_id;
            iss >> game_id;
            if (game_id.empty()) {
                std::cout << "[ERROR] Usage: join <game_id>" << std::endl;
            } else {
                send_join_game(sock, game_id);
            }
        }
        else if (cmd == "start") {
            if (g_game_id.empty()) {
                std::cout << "[ERROR] Not in a game. Use 'join <game_id>' first" << std::endl;
            } else {
                send_start_game(sock);
            }
        }
        else if (cmd == "info") {
            if (g_game_id.empty()) {
                std::cout << "[ERROR] Not in a game. Use 'join <game_id>' first" << std::endl;
            } else {
                send_get_game_info(sock);
            }
        }
        else if (cmd == "match") {
            int turn_id, symbol_id;
            iss >> turn_id >> symbol_id;
            if (iss.fail()) {
                std::cout << "[ERROR] Usage: match <turn_id> <symbol_id>" << std::endl;
            } else {
                send_match_symbol(sock, turn_id, symbol_id);
            }
        }
        else if (cmd == "auto") {
            if (!auto_refresh && !g_game_id.empty()) {
                auto_refresh = true;
                auto_thread = std::thread([sock, &auto_refresh]() {
                    while (auto_refresh && g_running) {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        if (auto_refresh && !g_game_id.empty()) {
                            send_get_game_info(sock);
                        }
                    }
                });
                std::cout << "[INFO] Auto-refresh enabled (every 1s)" << std::endl;
            } else {
                std::cout << "[ERROR] Already running or not in game" << std::endl;
            }
        }
        else if (cmd == "stop") {
            if (auto_refresh) {
                auto_refresh = false;
                if (auto_thread.joinable()) {
                    auto_thread.join();
                }
                std::cout << "[INFO] Auto-refresh stopped" << std::endl;
            }
        }
        else {
            std::cout << "[ERROR] Unknown command. Type 'help' for commands" << std::endl;
        }
        
        std::cout << ">>> ";
    }
    
    g_running = false;
    auto_refresh = false;
    
    if (auto_thread.joinable()) {
        auto_thread.join();
    }
    
    if (receiver.joinable()) {
        receiver.join();
    }
    
    close(sock);
    std::cout << "\n[INFO] Client disconnected. Goodbye!" << std::endl;
    
    return 0;
}
