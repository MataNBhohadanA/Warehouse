
#include "WareHouse.h"
#include <iostream>
using namespace std;



int main(int argc, char** argv) {
    WareHouse* backup = nullptr;
    string configFilePath = "configFileExample.txt"; // Relative path to the config file
    try {
        WareHouse wareHouse(configFilePath);
        wareHouse.start();
        // Your code to interact with the warehouse...
    } catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }

    return 0;
}
