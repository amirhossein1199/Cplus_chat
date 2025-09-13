#pragma once
#include "DataManager.h"

class Interface {
public:
    Interface(DataManager* dm);
    void startCLI();
    void runTests();
private:
    DataManager* dm;
};
