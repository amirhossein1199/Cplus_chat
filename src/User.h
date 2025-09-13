#pragma once
#include <string>
#include <ctime>

struct User {
    std::string username;
    std::string pass_hash;
    std::time_t created_at = 0;
    std::time_t last_login = 0;
    int unread_count = 0;
    int sent_count = 0;
};
