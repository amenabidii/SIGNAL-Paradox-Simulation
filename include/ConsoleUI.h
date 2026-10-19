#pragma once
#include "GameEngine.h"
#include <string>
// ConsoleUI owns only input/output. It never owns the crew or its dynamic records.
class ConsoleUI {
    GameEngine engine_;
    int readInt(const std::string& prompt,int low,int high);
    std::string readLine(const std::string& prompt);
    void showMenu()const;
    void createEntity();
    void modifyEntity();
    void addEvent();
    void specialAction(bool repairing);
    void investigate();
public:
    void run();
};
