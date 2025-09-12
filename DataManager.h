#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <vector>

class DataManager {
private:
    std::vector<std::string> messages;
public:
    void saveMessage(const std::string& message);
    std::vector<std::string> getMessages() const;
};

#endif // DATAMANAGER_H
