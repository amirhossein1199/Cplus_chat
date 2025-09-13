#include <iostream>
#include "DataManager.h"
#include "Interface.h"

int main(int argc, char** argv) {
    std::string datadir = "data";
    DataManager dm(datadir);
    dm.loadAll();

    Interface ui(&dm);
    if (argc>=2) {
        std::string arg = argv[1];
        if (arg == "run_tests") {
            ui.runTests();
            dm.saveAll();
            return 0;
        }
    }
    ui.startCLI();
    return 0;
}
