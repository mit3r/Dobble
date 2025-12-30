
#include <QByteArray>
#include <QTcpSocket>
#include <nlohmann/json.hpp>
#include <optional>

#pragma once

using json = nlohmann::json;
/**
 * @brief Attempts to extract one complete JSON packet from the buffer.
 *
 * @param buffer Reference to the receive buffer. If a complete packet is found,
 *               it will be removed from the buffer.
 * @return std::optional<json> The parsed JSON packet if complete, std::nullopt otherwise.
 */
inline std::optional<json> try_extract_packet(QByteArray& buffer) {
  // Need at least 9 bytes: 4 (preamble) + 4 (length) + 1 (minimum payload + newline)
  if (buffer.size() < 9) {
    return std::nullopt;
  }

  // Check preamble
  if (memcmp(buffer.data(), PREAMBLE, 4) != 0) {
    std::cerr << "[RECEIVER] Invalid preamble in buffer. Clearing buffer." << std::endl;
    buffer.clear();
    return std::nullopt;
  }

  // Check if we have the length field
  if (buffer.size() < 8) {
    return std::nullopt;
  }

  // Read the packet length
  uint32_t network_len;
  memcpy(&network_len, buffer.data() + 4, 4);
  uint32_t payload_len = ntohl(network_len);

  // Sanity check
  if (payload_len > 10 * 1024 * 1024) {
    std::cerr << "[RECEIVER] Packet too large: " << payload_len << std::endl;
    buffer.clear();
    return std::nullopt;
  }

  // Check if we have the complete packet: 4 (preamble) + 4 (length) + payload_len + 1 (newline)
  size_t total_packet_size = 4 + 4 + payload_len + 1;
  if (buffer.size() < static_cast<int>(total_packet_size)) {
    // Not enough data yet, wait for more
    return std::nullopt;
  }

  // Verify the newline suffix
  if (buffer[total_packet_size - 1] != '\n') {
    std::cerr << "[CLIENT ERROR] Invalid packet suffix (expected newline)." << std::endl;
    buffer.clear();
    return std::nullopt;
  }

  // Extract the JSON payload
  QByteArray json_data = buffer.mid(8, payload_len);

  // Remove the processed packet from buffer
  buffer.remove(0, total_packet_size);

  // Parse JSON
  try {
    return json::parse(json_data.begin(), json_data.end());
  } catch (const std::exception& e) {
    std::cerr << "[RECEIVER] Failed to parse JSON: " << e.what() << std::endl;
    return std::nullopt;
  }
}