#include <iostream>
#include <variant>
#include "CommandFactory.hpp"

struct CommandVisitor {
    
    void operator()(const SenderPingCommand& cmd) {
        std::cout << "[SENDER] Ping od klienta: " << cmd.client_id.value_or("?") << std::endl;
        std::cout << "[SENDER] Ping od klienta: " << cmd.data_obj->message << std::endl;    }
    void operator()(const ResponsePingCommand& cmd) {
        std::cout << "[RESPONSE] Pong od serwera: " << cmd.lobby_server_id.value_or("?") << std::endl;
    }

    void operator()(const SenderJoinGameCommand& cmd) {
        std::cout << "[SENDER] Chce dolaczyc do gry: " 
                  << (cmd.data_obj ? cmd.data_obj->game_id : "brak") << std::endl;
    }
    void operator()(const ResponseJoinGameCommand& cmd) {
        std::cout << "[RESPONSE] Status dolaczenia: " 
                  << (cmd.data_obj ? cmd.data_obj->status : "brak") << std::endl;
    }

    template<typename T>
    void operator()(const T& other) {
        std::cout << "[INFO] Obsluga innej komendy: " << other.command << std::endl;
    }
    void operator()(const std::monostate&) {
        std::cout << "[ERROR] Nieznana komenda lub blad." << std::endl;
    }
};

int main() {
    CommandFactory factory;

    std::string jsonSender = R"({
        "command": "getinfolobby",
        "client_id": "client-123",
        "data": { "message": "ping" }
    })";

 

    json j1 = json::parse(jsonSender);
    AnyCommand cmd1 = factory.get(j1["command"], j1);
    
    std::cout << "--- Test 1 (Sender) ---" << std::endl;
    std::visit(CommandVisitor{}, cmd1);

    std::string jsonResponse = R"({
        "command": "ping",
        "lobby_server_id": "server-main",
        "client_id": "client-123",
        "data": { "message": "pong" }
    })";

    json j2 = json::parse(jsonResponse);
    AnyCommand cmd2 = factory.get(j2["command"], j2);

    std::cout << "\n--- Test 2 (Response) ---" << std::endl;
    std::visit(CommandVisitor{}, cmd2);

    return 0;
}