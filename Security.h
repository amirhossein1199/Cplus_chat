#ifndef SECURITY_H
#define SECURITY_H

#include <string>

class Security {
public:
    static std::string encrypt(const std::string& message);
    static std::string decrypt(const std::string& encryptedMessage);
};

#endif // SECURITY_H
