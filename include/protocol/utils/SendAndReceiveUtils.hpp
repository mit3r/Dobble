#ifndef SEND_AND_RECEIVE_UTILS_HPP
#define SEND_AND_RECEIVE_UTILS_HPP

#include <vector>
#include <string>
#include <optional>
#include <nlohmann/json.hpp>


using json = nlohmann::json;
extern const char PREAMBLE[];
bool read_exact(int sock, char* buf, size_t n);
bool send_json_packet(int sock, const json& j);
std::optional<json> receive_json_packet(int sock);

#endif