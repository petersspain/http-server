class Socket {
public:
    Socket() = default;

    Socket(int socket_descr);

    void Set(int socket_descr);

    int Get() const { return socket_descr_; }

    ~Socket();
protected:
    int socket_descr_ = -1;
};

// class ClientSocket : public Socket {
// public:
//     ClientSocket(int socket_des)
// };