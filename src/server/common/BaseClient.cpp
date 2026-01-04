#include "BaseClient.hpp"

BaseClient::~BaseClient() {
    disconnect();
}

void BaseClient::disconnect() {
    if (sock_fd != -1) {
        close(sock_fd);
        sock_fd = -1;
    }
    connected = false;
    std::cout << "[Base] Rozłączono." << std::endl;
}

void BaseClient::setClientHandler(std::function<void(int)> handler) {
  client_handler_func = handler;
}


// bool BaseClient::isConnected() const {
//     return connected;
// }

UdsClient::UdsClient(const std::string& path) : socket_path(path) {}

void UdsClient::connect() {
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1) throw std::runtime_error("Błąd tworzenia gniazda UDS");

    int sock_buf_size = 5 * 1024 * 1024; 
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &sock_buf_size, sizeof(sock_buf_size));
    setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &sock_buf_size, sizeof(sock_buf_size));

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path.c_str(), sizeof(addr.sun_path) - 1);

    if (::connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sock_fd);
        throw std::runtime_error("Błąd łączenia z UDS: " + socket_path);
    }

    connected = true;
    std::cout << "[UDS] Połączono z " << socket_path << std::endl;
    client_handler_func(sock_fd);

}

TcpClient::TcpClient(const std::string& ip, int p) : ip_address(ip), port(p) {}

void TcpClient::connect() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) throw std::runtime_error("Błąd tworzenia gniazda TCP");

    int sock_buf_size = 5 * 1024 * 1024; 
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &sock_buf_size, sizeof(sock_buf_size));
    setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &sock_buf_size, sizeof(sock_buf_size));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); 
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (inet_pton(AF_INET, ip_address.c_str(), &addr.sin_addr) <= 0) {
        close(sock_fd);
        throw std::runtime_error("Nieprawidłowy adres IP");
    }

    if (::connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sock_fd);
        throw std::runtime_error("Błąd łączenia z TCP: " + ip_address);
    }

    connected = true;
    std::cout << "[TCP] Połączono z " << ip_address << ":" << port << std::endl;
}
