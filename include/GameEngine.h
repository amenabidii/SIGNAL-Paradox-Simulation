#pragma once
#include "GameState.h"
#include "MyStack.h"
#include <string>
#include <vector>

// GameEngine coordinates the simulation; it does not decide how individual
// researcher types perform their normal work (virtual dispatch handles that).
class GameEngine {
    GameState world_;
    MyStack<GameState> undoHistory_;
    std::vector<std::string> observerMemory_;
    std::vector<std::string> decisionHistory_;
    bool started_=false;
    void remember(const std::string& action);
    void applyStory(std::vector<std::string>& report);
public:
    GameState& world(){return world_;}
    const GameState& world()const{return world_;}
    int undoDepth()const{return undoHistory_.size();}
    const std::vector<std::string>& observerMemory()const{return observerMemory_;}
    void initializeScenario();
    int create(int type,const std::string& name,int health,int fatigue,int trust);
    int duplicate(int id);
    bool remove(int id);
    bool modify(int id,const std::string& attribute,int value);
    bool rename(int id,const std::string& name);
    bool addRecord(int id,const std::string& entry);
    bool replaceRecord(int id,int index,const std::string& entry);
    bool queueManualEvent(int kind,int targetId,const std::string& text);
    bool repairSpecial(int id,std::vector<std::string>& report);
    bool decodeSpecial(int id,std::vector<std::string>& report);
    bool undo(std::string& detail);
    std::vector<std::string> run(int count);
    std::string ending()const;
};
