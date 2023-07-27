#include <iostream>
#include <sstream>
#include <fstream>

#include <optional>

#include <string>
#include <string_view>

#include "server.hpp"
#include "msg_parser.hpp"

const std::string INDEX_PATH = "./front/index.html";
const size_t BUF_SIZE = 1024;

std::optional<std::string> GetResponse() {
    std::stringstream ss;
    ss << "HTTP/1.1 200 OK\r\nContent-Type: html; charset=utf-8\r\n";
    ss << "Connection: close\r\n";

    std::ifstream index_file(INDEX_PATH, std::ios::in | std::ios::ate);
    if (!index_file.is_open()) {
        return std::nullopt;
    }

    ss << "Content-Length: " << index_file.tellg() << "\r\n\n";
    index_file.seekg(0);

    std::string buf;
    buf.resize(BUF_SIZE);
    while (index_file.good()) {
        index_file.read(buf.data(), BUF_SIZE);
        ss << buf;
    }

    return ss.str();
}

// HTTP 0.9 - только метод GET
static const std::string POST_RESPONSE = "HTTP/1.1 200 OK\n";

static const std::string DELETE_RESPONSE = "HTTP/1.1 200 OK\
                                            \r\nConnection: close\n";

static const std::string BAD_REQUEST = "HTTP/1.1 400 Bad Request\n";

static const std::string INTERNAL_ERROR = "HTTP/1.1 500 Internal Error\n";

int main() {
    std::cout << "Server started!" << std::endl;
    // server
    Server server;
    // listen
    server.Listen(2);

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

        if (!parsed_message.has_value()) {
            // bad request
            if (server.SendMessage(client_socket.value(), BAD_REQUEST.data()) < 0) {
                break;
            }
        }

        
        // копирование строк не оч
        // стоит обработку запросов вынести в функции просто
        std::string response;
        switch (parsed_message->method) {
        case Method::POST:
            response = POST_RESPONSE;
            break;
        case Method::GET: {
            auto get_response = GetResponse();
            if (!get_response.has_value()) {
                response = INTERNAL_ERROR;
            }
            response = std::move(get_response.value());
            break;
        }
        case Method::DELETE:
            response = DELETE_RESPONSE;
            run = false;
            break;
        default:
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