#include <iostream>
#include <sstream>
#include <string_view>

#include "server.hpp"
#include "msg_parser.hpp"

// HTTP 0.9 - только метод GET
// 
static const std::string GET_RESPONSE = "HTTP/1.1 200 OK\
                                        \r\nContent-Type: text; charset=utf-8\
                                        \r\nContent-Length: 13\
                                        \r\n\nHello, USER!\n";

static const std::string POST_RESPONSE = "HTTP/1.1 200 OK\n";

static const std::string DELETE_RESPONSE = "HTTP/1.1 200 OK\
                                            \r\nConnection: close\n";

static const std::string BAD_REQUEST = "HTTP/1.1 400 Bad Request\n";

int main() {
    std::cout << "Server started!" << std::endl;
    // server
    Server server;
    // listen
    server.Listen(1);

    bool run = true;

    while (run) {
        // accept
        auto client_socket = server.AcceptConnection();
        if (!client_socket.has_value()) {
            break;
        }

        // recv
        auto received = server.RecieveMessage(client_socket.value());
        if (!received.has_value()) {
            break;
        }
        std::cout << "\nRecieved message:\n" << received.value() << std::endl;

        auto parsed_message = ParseMessage(received.value());

        std::string_view response;
        switch (parsed_message.method) {
        case Method::POST:
            response = POST_RESPONSE;
            break;
        case Method::GET:
            response = GET_RESPONSE;
            break;
        case Method::DELETE:
            response = DELETE_RESPONSE;
            run = false;
            break;
        case Method::INVALID:
            response = BAD_REQUEST;
            break;
        }

        // send
        if (server.SendMessage(client_socket.value(), response.data()) < 0) {
            break;
        }
    }

    // Изучить хедеры http и начать как то их использовать
    // Например: 
    // 1. Авторизоваться в серваке
    //  (тут можно накрутить какой то интерфейс на джаваскрипте!!)
    //  1.1 Выводить коннект в другой поток мб
    //  1.2 Держать этот коннект какое то время, чтобы принимать новые сообщения
    //      Кстати коннект от браузера не закрывается, хотя от инсомнии как будто закрылся
    // 2. Отдать какую то админскую команду
    //  Например: открой с именем А в директории Б

    std::cout << "Server stopped!" << std::endl;
    return 0;
}