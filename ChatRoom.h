#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "User.h"
#include "Message.h"

class ChatRoom {
private:
    int roomId;
    std::string roomName;

    std::unordered_set<int> memberIds;   // keep track of user ids to avoid duplicates
    std::vector<User*> members;          // list of members in the room
    std::vector<Message> messages;       // chat history

public:
    ChatRoom(int id, const std::string& name);

    // user management
    bool addUser(User* user);
    bool removeUser(User* user);
    bool kickUser(User* user);
    bool hasUser(int userId) const;

    // messages
    void addMessage(const Message& msg);
    const std::vector<Message>& getHistory() const;
    std::vector<Message> searchMessages(const std::string& keyword) const;
    std::vector<Message> lastN(size_t n) const;

    // basic info
    std::string getName() const;
    int getId() const;
    size_t memberCount() const;
};
