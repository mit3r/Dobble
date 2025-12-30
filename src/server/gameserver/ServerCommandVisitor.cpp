#include "ServerCommandVisitor.hpp"
#include "GameStateManager.hpp"

std::string gen_random(const int len)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

extern GameStateManager g_game_server;

ServerCommandVisitor::ServerCommandVisitor(int sock, ServersCommandFactory &f)
    : client_sock(sock), factory(f) {}

ServerCommandVisitor::ServerCommandVisitor(int sock, GameCommandFactory &f)
    : client_sock(sock), factory(f) {}

void ServerCommandVisitor::sendErrorResponse(
    const std::string &commandName,
    const std::string &code,
    const std::string &message)
{
    ErrorResponse e;
    e.command = commandName;
    e.lobby_server_id = "MainServer_v1";

    ErrorStruct err;
    err.code = std::stoi(code);
    err.message = message;
    e.error = err;

    json j = e;

    send_json_packet(client_sock, j);
}


void ServerCommandVisitor::operator()(const SenderJoinGameCommand &cmd){
    std::optional<std::string> client_id = cmd.client_id;
    std::cout << "[CMD] Klient chce dolaczyc do gry: " << (cmd.data_obj ? cmd.data_obj->game_id : "brak") << std::endl;


    g_game_server.addPlayer(client_id.has_value() ? std::stoi(client_id.value()) : -1,
                               cmd.data_obj ? cmd.data_obj->game_id : "unknown");






};
void ServerCommandVisitor::operator()(const SenderSendGameInfoCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseSendGameInfoCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseGameClientPingCommand &cmd){};
void ServerCommandVisitor::operator()(const SenderGameClientPingCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseJoinGameCommand &cmd){};
void ServerCommandVisitor::operator()(const SenderLeaveRoomCommand &cmd){};
void ServerCommandVisitor::operator()(const ResponseLeaveRoomCommand &cmd){};








void ServerCommandVisitor::operator()(const std::monostate &)
{
    std::cerr << "[ERROR] Nie udało się rozpoznać komendy w JSON." << std::endl;
};


