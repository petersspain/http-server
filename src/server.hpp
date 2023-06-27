#include "socket.hpp"

#include <string>
#include <optional>

class Server : public Socket {
public:
    Server();

    Server(const std::string& hostname, const std::string& servname);

    int Listen(int connections_queue_size);

    std::optional<Socket> AcceptConnection();

    std::optional<std::string> RecieveMessage(const Socket& client_socket);

    int SendMessage(const Socket& client_socket, const std::string& message);
private:
    static const int max_client_buffer_size_ = 1024;
};