#pragma once
#include <string>
#include <ctime>

struct Message {
    std::string from;
    std::string to; // username or "room:<id>"
    std::time_t sent_at = 0;
    std::string content;
    bool read = false;
};
