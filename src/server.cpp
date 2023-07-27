#include "server.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <exception>
#include <type_traits>

Server::Server() : Server("127.0.0.1", "8000") {}

// hostname, servname можно вынести в поля класса
// и например делать конект, когда хочется
Server::Server(const std::string& hostname, const std::string& servname) {
    // Тут можно заполнять через параметры
    addrinfo hints;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* addr;
    // getting http address
    if (int error = getaddrinfo(hostname.c_str(), servname.c_str(), &hints, &addr)) {
        auto err_str = gai_strerror(error);
        std::cout << err_str << std::endl;
        throw std::runtime_error(err_str);
    }

    // assume we got first address correctly
    int socket_descr = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (socket_descr == -1) {
        freeaddrinfo(addr);
        auto err_str = strerror(errno);
        std::cout << "Failed to create socket: " << err_str << std::endl;
        throw std::runtime_error(err_str);
    }

    // Think about remove this, cause server has no need to restart so much time
    int opt = 1;
    if (setsockopt(socket_descr, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        freeaddrinfo(addr);
        auto err_str = strerror(errno);
        std::cout << "Failed to set SO_REUSEADDR for socket: " << err_str << std::endl;
        throw std::runtime_error(err_str);
    }

    socket_descr_ = socket_descr;
    
    if (bind(socket_descr, addr->ai_addr, addr->ai_addrlen) < 0) {
        freeaddrinfo(addr);
        auto err_str = strerror(errno);
        std::cout << "Failed to bind address: " << err_str << std::endl;
        close(socket_descr_);
        throw std::runtime_error(err_str);
    }
    freeaddrinfo(addr);
}

int Server::Listen(int connections_queue_size) {
    return listen(socket_descr_, connections_queue_size);
}

std::optional<Socket> Server::AcceptConnection() {
    // проверка на socket_descr_ == -1
    std::cout << "Waiting for connection" << std::endl;
    int client_socket = accept(socket_descr_, nullptr, nullptr);
    if (client_socket < 0) {
        std::cout << "Accept error: " << strerror(errno) << std::endl;
        return std::nullopt;
    }
    // здесь должно сработать NRVO
    return client_socket;
}

std::optional<std::string> Server::RecieveMessage(const Socket& client_socket) {
    char client_buffer[max_client_buffer_size_];

    std::cout << "Waiting for message from client" << std::endl;
    int result = recv(client_socket.Get(), client_buffer, max_client_buffer_size_, 0);

    if (result < 0) {
        std::cout << "Recv error: " << strerror(errno) << std::endl;
        return std::nullopt;
    }
    else if (result == 0) {
        std::cout << "Connection closed by peer" << std::endl;
        return std::nullopt;
    }
    else {
        client_buffer[result] = '\0';
        return client_buffer;
    }
}

int Server::SendMessage(const Socket& client_socket, const std::string& message) {
    int res = send(client_socket.Get(), message.c_str(), message.size(), 0);
    if (res < 0) {
        std::cout << "Send error: " << strerror(errno) << std::endl;
    }
    return res;
}
