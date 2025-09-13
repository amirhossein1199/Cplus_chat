#pragma once
#include <string>
#include <vector>
#include "Message.h"

struct ChatRoom {
    int id;
    std::string name;
    std::string owner;
    std::vector<std::string> members; // usernames
    std::vector<Message> history;
};
