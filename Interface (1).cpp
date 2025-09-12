#include "Interface.h"
#include <iostream>

void Interface::sendMessage(const std::string& message) {
    std::string encrypted = Security::encrypt(message);
    dataManager.saveMessage(encrypted);
    std::cout << "Message sent: " << encrypted << std::endl;
}

void Interface::showMessages() const {
    auto messages = dataManager.getMessages();
    std::cout << "Stored Messages:" << std::endl;
    for (const auto& msg : messages) {
        std::cout << "- " << Security::decrypt(msg) << std::endl;
    }
}
