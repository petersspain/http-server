#include <string_view>
#include <string>
#include <optional>

enum class Method {
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
    std::string uri;
    std::string version;
};

std::optional<Message> ParseMessage(std::string_view message);