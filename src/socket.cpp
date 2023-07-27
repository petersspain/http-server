#include "socket.hpp"

#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

Socket::Socket(int socket_descr)
    : socket_descr_(socket_descr) {}

// тут возможно стоит поставить точку и посмотреть будет ли работать NRVO
Socket::~Socket() {
    if (socket_descr_ != -1) {
        close(socket_descr_);
    }
}

void Socket::Set(int socket_descr) {
    if (socket_descr_ != -1) {
        close(socket_descr_);
    }
    socket_descr_ = socket_descr;
}