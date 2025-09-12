#include "DataManager.h"

void DataManager::saveMessage(const std::string& message) {
    messages.push_back(message);
}

std::vector<std::string> DataManager::getMessages() const {
    return messages;
}
