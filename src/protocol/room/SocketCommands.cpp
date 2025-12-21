#pragma once 

#include <string>
#include <optional>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ConfigData {
    std::string param1;
    int param2;
};

struct RoomServerReceivedMessage {
    std::string command;
    std::string server_id;
    ConfigData data;               
    std::optional<std::string> error; 
};

struct RoomClientReceivedMessage {
    std::string command;
    std::string client_id;
    ConfigData data;               
    std::optional<std::string> error; 
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ConfigData, param1, param2)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoomServerReceivedMessage, command, server_id, data, error)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoomClientReceivedMessage, command, client_id, data, error)