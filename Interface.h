#ifndef INTERFACE_H
#define INTERFACE_H

#include "DataManager.h"
#include "Security.h"

class Interface {
private:
    DataManager dataManager;
public:
    void sendMessage(const std::string& message);
    void showMessages() const;
};

#endif // INTERFACE_H
