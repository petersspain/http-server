#include "msg_parser.hpp"

#include <utility>

std::optional<Method> ParseMethod(std::string_view method) {
    if (method.size() < 3) {
        return std::nullopt;
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
    return std::nullopt;
}

std::optional<Message> ParseMessage(std::string_view message) {
    Message result;
    const auto npos = message.npos;

    // get method
    size_t pos = message.find(' ');
    if (pos == npos) {
        return std::nullopt;
    }
    auto method = ParseMethod(message.substr(0, pos));
    if (!method.has_value()) {
        return std::nullopt;
    }
    result.method = std::move(method.value());
    
    // get uri
    message.remove_prefix(pos + 1);
    if (pos = message.find(' ');
        pos == npos) {
        return std::nullopt;
    }
    result.uri = message.substr(0, pos);
    
    // get http version
    message.remove_prefix(pos + 1);
    if (pos = message.find("HTTP/"); 
        pos == npos) {
        return std::nullopt;
    }
    pos += 5;
    auto end = message.find('\n');
    result.version = message.substr(pos, end - pos);

    // parse headers (find \n\n and take this string to parse headers)
    return result;
}