#include "msg_parser.hpp"

Method ParseMethod(std::string_view method) {
    if (method.size() < 3) {
        return Method::INVALID;
    }
    switch (method[0]) {
    case 'O':
        if (method.size() == 7 && method == "OPTIONS") {
            return Method::OPTIONS;
        }
        break;
    case 'G':
        if (method.size() == 3 && method == "GET") {
            return Method::GET;
        }
        break;
    case 'H':
        if (method.size() == 5 && method == "HEAD") {
            return Method::HEAD;
        }
        break;
    case 'P':
        switch (method.size()) {
            case 3:
                if (method == "PUT") {
                    return Method::PUT;
                }
                break;
            case 4:
                if (method == "POST") {
                    return Method::POST;
                }
                break;
            case 5:
                if (method == "PATCH") {
                    return Method::PATCH;
                }
                break;
        }
        break;
    case 'D':
        if (method.size() == 6 && method == "DELETE") {
            return Method::DELETE;
        }
        break;
    case 'T':
        if (method.size() == 5 && method == "TRACE") {
            return Method::TRACE;
        }
        break;
    case 'C':
        if (method.size() == 7 && method == "CONNECT") {
            return Method::CONNECT;
        }
        break;
    }
    return Method::INVALID;
}

Message ParseMessage(std::string_view message) {
    Method method = Method::INVALID;
    auto pos = message.find(' ');
    if (pos != message.npos) {
        method = ParseMethod(message.substr(0, pos));
    }
    return { method };
}