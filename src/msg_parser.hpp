#include <string_view>

enum class Method {
    INVALID = -1,
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    PATCH,
    DELETE,
    TRACE,
    CONNECT,
};

struct Message {
    Method method;
};

Message ParseMessage(std::string_view message);