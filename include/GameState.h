#pragma once
#include "Entity.h"
#include "MyQueue.h"
#include <memory>
#include <vector>
#include <string>
#include <ostream>

enum class EventType {EquipmentAlarm=1,SecurityIncident=2,MedicalAlert=3,Transmission=4};
struct Event {
    int id=0;
    EventType type=EventType::EquipmentAlarm;
    int targetId=0;
    std::string note;
};

// GameState owns every polymorphic entity and the pending queue. Deep copying it
// is what makes deleted people and their private records come back correctly on undo.
class GameState {
    std::vector<std::unique_ptr<Entity>> entities_;
    MyQueue<Event> pending_;
    int nextId_=1;
    int nextEventId_=1;
    int turn_=0;
    int receiver_=90;
    int security_=90;
    int research_=0;
    int decoded_=0;
    int healthAlerts_=0;
    std::vector<std::string> evidence_;
public:
    GameState()=default;
    GameState(const GameState& other);
    GameState& operator=(const GameState& other);
    int turn()const{return turn_;}
    int receiver()const{return receiver_;}
    int security()const{return security_;}
    int research()const{return research_;}
    int decoded()const{return decoded_;}
    int healthAlerts()const{return healthAlerts_;}
    const std::vector<std::string>& evidence()const{return evidence_;}
    const std::vector<std::unique_ptr<Entity>>& entities()const{return entities_;}
    int create(int type,const std::string& name,int health,int fatigue,int trust);
    int duplicate(int id);
    bool remove(int id);
    Entity* find(int id);
    const Entity* find(int id)const;
    bool modify(int id,const std::string& attribute,int value);
    void addEvidence(const std::string& entry);
    void damageReceiver(int amount);
    void repairReceiver(int amount);
    void changeSecurity(int amount);
    void changeResearch(int amount);
    void addDecoded(){++decoded_;}
    void addHealthAlert(){++healthAlerts_;}
    void queueEvent(EventType type,int targetId,const std::string& note);
    const MyQueue<Event>& pending()const{return pending_;}
    void advanceTurn(std::vector<std::string>& report);
    void printStatus(std::ostream& out)const;
    void printEntities(std::ostream& out)const;
    void printEvents(std::ostream& out)const;
    void printEvidence(std::ostream& out)const;
};
