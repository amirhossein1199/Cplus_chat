#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <ctime>
#include "json.hpp"   // کتابخونه JSON
using json = nlohmann::json;

class Message {
private:
    std::string sender;
    std::string content;
    std::time_t timestamp;

public:
    Message(const std::string& s, const std::string& c, std::time_t t);
    Message(); // کانستراکتور پیش‌فرض

    // Getterها
    std::string getSender() const;
    std::string getContent() const;
    std::time_t getTimestamp() const;

    // JSON تبدیل به
    json toJson() const;
    static Message fromJson(const json& j);
};

#endif
