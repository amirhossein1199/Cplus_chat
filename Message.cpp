#include "Message.h"

Message::Message(const std::string& s, const std::string& c, std::time_t t)
    : sender(s), content(c), timestamp(t) {}

Message::Message() : sender(""), content(""), timestamp(std::time(nullptr)) {}

std::string Message::getSender() const { return sender; }
std::string Message::getContent() const { return content; }
std::time_t Message::getTimestamp() const { return timestamp; }

json Message::toJson() const {
    return json{
        {"sender", sender},
        {"content", content},
        {"timestamp", timestamp}
    };
}

Message Message::fromJson(const json& j) {
    return Message(
        j.at("sender").get<std::string>(),
        j.at("content").get<std::string>(),
        j.at("timestamp").get<std::time_t>()
    );
}
