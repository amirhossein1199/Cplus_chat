#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "Message.h"
#include "ChatRoom.h"

class DataManager {
public:
    DataManager(const std::string &dataDir = "data");
    bool loadAll();
    bool saveAll();

    // users
    bool addUser(const User &u);
    bool deleteUser(const std::string &username);
    User* findUser(const std::string &username);
    std::vector<std::string> getAllUsernames();

    // messages
    void addMessage(const Message &m);
    std::vector<Message> getInbox(const std::string &username, int limit=-1);
    std::vector<Message> getRoomHistory(int roomId, int limit=-1);

    // rooms
    int createRoom(const std::string &name, const std::string &owner);
    bool inviteToRoom(int roomId, const std::string &username);
    bool leaveRoom(int roomId, const std::string &username);

private:
    std::string dir;
    std::vector<User> users;
    std::vector<Message> messages;
    std::vector<ChatRoom> rooms;

    // internal helpers
    void loadUsers();
    void saveUsers();
    void loadMessages();
    void saveMessages();
    void loadRooms();
    void saveRooms();
};
