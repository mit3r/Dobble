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
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// --- STAŁE PROTOKOŁU ---
const unsigned char PREAMBLE[] = {0xCA, 0xFE, 0xBE, 0xBE};
const char SUFFIX = '\n'; 

// --- Zmienne globalne i pomocnicze ---
std::mutex g_cout_mutex;
bool g_running = true; 

/**
 * @brief Odbiera dokładnie 'len' bajtów danych z socketu.
 */
bool read_exact(int socket_fd, char* buffer, size_t len) {
    size_t total_received = 0;
    while (total_received < len && g_running) {
        ssize_t received = recv(socket_fd, buffer + total_received, len - total_received, 0);
        if (received <= 0) {
            if (received == 0) {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cout << "\n[INFO] Server closed connection." << std::endl;
            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cerr << "read_exact error" << std::endl;
            }
            g_running = false;
            return false;
        }
        total_received += received;
    }
    return true;
}

/**
 * @brief Wysyła dokładnie 'len' bajtów danych przez socket.
 */
bool write_exact(int socket_fd, const char* buffer, size_t len) {
    size_t total_sent = 0;
    while (total_sent < len) {
        ssize_t sent = send(socket_fd, buffer + total_sent, len - total_sent, 0);
        if (sent <= 0) {
            if (sent == 0 || (errno != EAGAIN && errno != EWOULDBLOCK)) {
                std::lock_guard<std::mutex> lock(g_cout_mutex);
                std::cerr << "write_exact error" << std::endl;
                return false;
            }
        } else {
            total_sent += sent;
        }
    }
    return true;
}

// --- FUNKCJE PROTOKOŁU ---

/**
 * @brief Wysyła pakiet JSON z Magic Number, długością i sufiksem.
 */
bool send_json_packet(int socket_fd, const json& j) {
    std::string data = j.dump();
    uint32_t length = data.length();
    
    // 1. Wysyłanie stałej preambuły (Magic Number)
if (!write_exact(socket_fd, reinterpret_cast<const char*>(PREAMBLE), 4)) return false;
    // 2. Wysyłanie długości (Network Byte Order)
    uint32_t net_length = htonl(length); 
    if (!write_exact(socket_fd, reinterpret_cast<const char*>(&net_length), sizeof(net_length))) return false;

    // 3. Wysyłanie danych JSON
    if (!write_exact(socket_fd, data.c_str(), length)) return false;

    // 4. Wysyłanie sufiksu (\n)
    if (!write_exact(socket_fd, &SUFFIX, 1)) return false;

    return true;
}

/**
 * @brief Odbiera pakiet JSON, weryfikując Magic Number i sufiks.
 */
std::optional<json> receive_json_packet(int sock) {
    char header[4];
    if (!read_exact(sock, header, 4)) return std::nullopt;

    // Weryfikacja Magic Number
    if (memcmp(header, PREAMBLE, 4) != 0) {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        fprintf(stderr, "\n[PROTOCOL ERROR] Invalid Magic Number!\n");
        fprintf(stderr, "EXPECTED: 0x%02X%02X%02X%02X\n", 
            (unsigned char)PREAMBLE[0], (unsigned char)PREAMBLE[1], (unsigned char)PREAMBLE[2], (unsigned char)PREAMBLE[3]);
        fprintf(stderr, "RECEIVED: 0x%02X%02X%02X%02X\n", 
            (unsigned char)header[0], (unsigned char)header[1], (unsigned char)header[2], (unsigned char)header[3]);
        g_running = false;
        return std::nullopt;
    }

    // Odbiór długości
    uint32_t network_len;
    if (!read_exact(sock, (char*)&network_len, 4)) return std::nullopt;
    
    uint32_t len = ntohl(network_len);
    if (len > 10 * 1024 * 1024) { // Limit bezpieczeństwa
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cerr << "[PROTOCOL ERROR] Packet too large: " << len << " bytes." << std::endl;
        g_running = false;
        return std::nullopt;
    }

    // Odbiór danych JSON
    std::vector<char> buffer(len);
    if (!read_exact(sock, buffer.data(), len)) return std::nullopt;

    // Odbiór sufiksu
    char suffix;
    if (!read_exact(sock, &suffix, 1)) return std::nullopt;
    if (suffix != SUFFIX) {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cerr << "[PROTOCOL ERROR] Invalid suffix (expected '\\n')." << std::endl;
        g_running = false;
        return std::nullopt;
    }

    try {
        return json::parse(buffer.begin(), buffer.end());
    } catch (const json::parse_error& e) {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cerr << "\n[ERROR] JSON parse error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

// --- POZOSTAŁA LOGIKA KLIENTA (BEZ KOMENTARZY) ---

void receive_thread(int sock) {
    while (g_running) {
        std::optional<json> response = receive_json_packet(sock);
        if (response.has_value()) {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "\n\n<<< OTRZYMANO ODPOWIEDŹ >>>" << std::endl;
            std::cout << response->dump(4) << std::endl;
            std::cout << ">>> Wprowadź nową komendę: " << std::flush;
        } else if (!g_running) {
            break;
        }
    }
}

std::optional<json> get_predefined_command(int choice) {
    static const std::string CLIENT_ID = "interactive_client_001"; 

    switch (choice) {
        case 1: { 
            std::string nickname;
            std::cout << "Podaj swój Nickname: ";
            std::cin >> nickname;
            return json::parse(R"({"command": "login","client_id": ")" + CLIENT_ID + R"(","data": {"nickname": ")" + nickname + R"("}})");
        }
        case 2: { 
            return json::parse(R"({"command": "ping","client_id": "test_client_001","data": {"message": "Are you alive?"}})");
        }
        case 3: { 
            return json::parse(R"({"command": "getinfolobby","client_id": "test_client_001","data": {"page_number": 1}})");
        }
        case 4: { 
            std::string game_id;
            std::cout << "Podaj ID gry do dołączenia (np. 'game_1'): ";
            std::cin >> game_id;
            return json::parse(R"({"command": "join_game","client_id": "test_client_001","data": {"game_id": ")" + game_id + R"("}})");
        }
        case 5: { // CREATE_LOBBY
            std::string game_name;
            std::cout << "Podaj nazwę nowego pokoju: ";
            std::cin.ignore(); // Clear buffer
            std::getline(std::cin, game_name);
            return json::parse(R"({"command": "create_lobby","client_id": ")" + CLIENT_ID + R"(","data": {"game_name": ")" + game_name + R"("}})");
        }
        default:
            return std::nullopt;
    }
}

std::optional<json> get_manual_command() {
    std::cout << "Wklej swój pakiet JSON (zakończ dwukrotnym wciśnięciem Enter na pustej linii):\n";
    std::string line;
    std::string full_json_str;
    
    std::cin.ignore(10000, '\n'); 
    while (std::getline(std::cin, line) && !line.empty()) {
        full_json_str += line + '\n'; 
    }

    if (full_json_str.empty()) {
        std::lock_guard<std::mutex> lock_again(g_cout_mutex);
        std::cerr << "Nie wprowadzono żadnych danych." << std::endl;
        return std::nullopt;
    }

    try {
        return json::parse(full_json_str);
    } catch (const json::parse_error& e) {
        std::lock_guard<std::mutex> lock_again(g_cout_mutex);
        std::cerr << "Błąd parsowania JSON: " << e.what() << std::endl;
        return std::nullopt;
    }
}

int main() {
    const char* SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 1500;
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Nieprawidłowy adres" << std::endl;
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    std::cout << "Połączono z serwerem na " << SERVER_IP << ":" << SERVER_PORT << std::endl;
    
    std::thread receiver(receive_thread, sock);

    while (g_running) {
        std::lock_guard<std::mutex> lock(g_cout_mutex);
        std::cout << "\n==============================================" << std::endl;
        std::cout << "Wybierz opcję (pełny protokół CAFEBEBE):" << std::endl;
        std::cout << "1. LOGIN" << std::endl;
        std::cout << "2. PING" << std::endl;
        std::cout << "3. GETLOBBYINFO" << std::endl;
        std::cout << "4. JOIN_GAME" << std::endl;
        std::cout << "5. CREATE_LOBBY (Utwórz nowy pokój)" << std::endl;
        std::cout << "6. WYŚLIJ WŁASNY JSON" << std::endl;
        std::cout << "0. WYJDŹ" << std::endl;
        std::cout << "Wybór: " << std::flush;
        
        int choice;
        std::optional<json> command_to_send = std::nullopt;
        
        lock.~lock_guard(); 
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::lock_guard<std::mutex> lock_again(g_cout_mutex);
            std::cerr << "Nieprawidłowy wybór. Spróbuj ponownie." << std::endl;
            continue;
        }
        
        if (choice >= 1 && choice <= 5) {
             std::cin.ignore(10000, '\n'); 
        }

        if (choice == 0) {
            g_running = false;
            break;
        } else if (choice >= 1 && choice <= 5) {
            command_to_send = get_predefined_command(choice);
        } else if (choice == 6) {
            command_to_send = get_manual_command();
        } else {
            std::lock_guard<std::mutex> lock_again(g_cout_mutex);
            std::cerr << "Nieznana opcja. Spróbuj ponownie." << std::endl;
            continue;
        }

        if (command_to_send.has_value()) {
            std::lock_guard<std::mutex> lock_again(g_cout_mutex);
            std::cout << "--- Wysyłanie pakietu: " << command_to_send->dump(4) << " ---" << std::endl;
            send_json_packet(sock, command_to_send.value());
        }
    }
    
    if (receiver.joinable()) {
        shutdown(sock, SHUT_RDWR); 
        receiver.join();
    }
    close(sock);
    std::cout << "Klient zakończył pracę." << std::endl;

    return 0;
}