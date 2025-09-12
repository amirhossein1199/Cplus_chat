#include "Security.h"

std::string Security::encrypt(const std::string& message) {
    std::string result = message;
    for (char& c : result) c += 1;
    return result;
}

std::string Security::decrypt(const std::string& encryptedMessage) {
    std::string result = encryptedMessage;
    for (char& c : result) c -= 1;
    return result;
}
