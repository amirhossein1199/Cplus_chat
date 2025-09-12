#include "Interface.h"
#include <iostream>

int main() {
    Interface chat;
    chat.sendMessage("Hello Team!");
    chat.sendMessage("This is a test message.");
    chat.showMessages();
    return 0;
}
