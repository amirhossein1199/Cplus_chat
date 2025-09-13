#include "Security.h"
#include <functional>
#include <sstream>

namespace Security {
    std::string hash_password(const std::string &pwd) {
        // Lightweight portable placeholder
        std::size_t h = std::hash<std::string>{}(pwd);
        std::ostringstream oss;
        oss << std::hex << h;
        return oss.str();
    }
}
