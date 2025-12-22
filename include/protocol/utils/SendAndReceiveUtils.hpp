#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <nlohmann/json.hpp>

#pragma once

const char PREAMBLE[] = {(char)0xCA, (char)0xFE, (char)0xBE, (char)0xBE};

bool inline read_exact(int sock, char* buf, size_t n) {
  size_t total_read = 0;
  while (total_read < n) {
    ssize_t r = recv(sock, buf + total_read, n - total_read, 0);
    if (r <= 0) return false;
    total_read += r;
  }
  return true;
}

bool inline send_json_packet(int sock, const json& j) {
  std::string payload = j.dump();
  uint32_t len = static_cast<uint32_t>(payload.size());
  uint32_t net_len = htonl(len);

  std::vector<char> packet;
  packet.reserve(4 + 4 + len + 1);

  packet.insert(packet.end(), std::begin(PREAMBLE), std::end(PREAMBLE));

  char* len_bytes = reinterpret_cast<char*>(&net_len);
  packet.insert(packet.end(), len_bytes, len_bytes + 4);

  packet.insert(packet.end(), payload.begin(), payload.end());

  packet.push_back('\n');

  ssize_t sent = send(sock, packet.data(), packet.size(), 0);
  return sent == (ssize_t)packet.size();
}

inline std::optional<json> receive_json_packet(int sock) {
  char header[4];
  if (!read_exact(sock, header, 4)) return std::nullopt;

  if (memcmp(header, PREAMBLE, 4) != 0) {
    fprintf(stderr, "PREAMBLE: 0x%02X%02X%02X%02X\n",
            (unsigned char)PREAMBLE[0],
            (unsigned char)PREAMBLE[1],
            (unsigned char)PREAMBLE[2],
            (unsigned char)PREAMBLE[3]);
    fprintf(stderr, "header: 0x%02X%02X%02X%02X\n",
            (unsigned char)header[0],
            (unsigned char)header[1],
            (unsigned char)header[2],
            (unsigned char)header[3]);
    std::cerr << "[PROTOCOL ERROR] Invalid Magic Number!" << std::endl;
    return std::nullopt;
  }

  uint32_t network_len;
  if (!read_exact(sock, (char*)&network_len, 4)) return std::nullopt;

  uint32_t len = ntohl(network_len);
  if (len > 10 * 1024 * 1024) return std::nullopt;

  std::vector<char> buffer(len);
  if (!read_exact(sock, buffer.data(), len)) return std::nullopt;

  char suffix;
  if (!read_exact(sock, &suffix, 1)) return std::nullopt;
  if (suffix != '\n') return std::nullopt;

  try {
    return json::parse(buffer.begin(), buffer.end());
  } catch (...) {
    return std::nullopt;
  }
}
