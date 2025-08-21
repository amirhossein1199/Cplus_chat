#include "ChatRoom.h"
#include <algorithm>
#include <iostream>

ChatRoom::ChatRoom(int id, const std::string& name)
    : roomId(id), roomName(name) {}

// add a user if not already in the room
bool ChatRoom::addUser(User* user) {
    if (!user) return false;
    int uid = user->getId();
    if (memberIds.insert(uid).second) {
        members.push_back(user);
        return true;
    }
    return false;
}

// remove a user from the room
bool ChatRoom::removeUser(User* user) {
    if (!user) return false;
    int uid = user->getId();
    if (memberIds.erase(uid) == 0) return false;

    auto it = std::remove(members.begin(), members.end(), user);
    if (it != members.end()) {
        members.erase(it, members.end());
        return true;
    }
    return false;
}

// kick a user and log the action
bool ChatRoom::kickUser(User* user) {
    bool removed = removeUser(user);
    if (removed && user) {
        std::cout << user->getName() << " was kicked from " << roomName << std::endl;
    }
    return removed;
}

// check if a user is already in the room
bool ChatRoom::hasUser(int userId) const {
    return memberIds.find(userId) != memberIds.end();
}

// add a message to the room history
void ChatRoom::addMessage(const Message& msg) {
    messages.push_back(msg);
}

// return all messages
const std::vector<Message>& ChatRoom::getHistory() const {
    return messages;
}

// search for messages containing a keyword
std::vector<Message> ChatRoom::searchMessages(const std::string& keyword) const {
    std::vector<Message> result;
    for (const auto& msg : messages) {
        if (msg.getContent().find(keyword) != std::string::npos) {
            result.push_back(msg);
        }
    }
    return result;
}

// get the last N messages from history
std::vector<Message> ChatRoom::lastN(size_t n) const {
    std::vector<Message> out;
    if (messages.empty()) return out;
    size_t start = (messages.size() > n) ? messages.size() - n : 0;
    out.insert(out.end(), messages.begin() + start, messages.end());
    return out;
}

// getters
std::string ChatRoom::getName() const { return roomName; }
int ChatRoom::getId() const { return roomId; }
size_t ChatRoom::memberCount() const { return members.size(); }
