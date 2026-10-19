#pragma once
#include "DynamicRecords.h"
#include <memory>
#include <string>
#include <vector>

class GameState;

// One general type for every object in the simulation. Keeping the ID private
// means menu input cannot accidentally give two entities the same identity.
class Entity {
    int id_ = 0;
    std::string name_;
    bool active_ = true;
public:
    Entity();
    Entity(int id,const std::string& name);
    Entity(const Entity&) = default;
    Entity& operator=(const Entity&) = default;
    virtual ~Entity() = default; // deletion through Entity* must run the derived destructor
    int id() const {return id_;}
    const std::string& name() const{return name_;}
    bool active() const{return active_;}
    void setName(const std::string& name);
    void setActive(bool value){active_=value;}
    void assignId(int id);
    virtual std::string kind() const=0;
    virtual std::unique_ptr<Entity> clone() const=0;
    virtual void performTurn(GameState& world,std::vector<std::string>& report)=0;
    virtual void printDetails(std::ostream& output) const=0;
    virtual bool updateAttribute(const std::string& attribute,int value);
    virtual void addRecord(const std::string& value)=0;
    virtual const DynamicRecords& records() const=0;
    virtual DynamicRecords& records()=0;
};

// Researchers share health, trust, fatigue and assignments, regardless of their job.
// Concrete researchers supply their own turn rules and own their own DynamicRecords.
class Researcher : public Entity {
    int health_=100;
    int fatigue_=0;
    int trust_=60;
    std::string assignment_="Routine";
public:
    Researcher();
    Researcher(int id,const std::string& name,int health,int fatigue,int trust);
    int health()const{return health_;}
    int fatigue()const{return fatigue_;}
    int trust()const{return trust_;}
    const std::string& assignment() const{return assignment_;}
    void changeHealth(int change);
    void changeFatigue(int change);
    void changeTrust(int change);
    bool updateAttribute(const std::string& key,int value) override;
    void printDetails(std::ostream& out) const override;
    void setAssignment(const std::string& value){assignment_=value;}
protected:
    void finishTurn(const std::string& action,std::vector<std::string>& report)const;
};

// Engineering, decoding, astronomy, security, medicine and communications act
// differently each turn. These distinct subclasses are not just name labels.
class Engineer : public Researcher {
    DynamicRecords log_;
public:
    Engineer();
    Engineer(int id,const std::string& name,int health=100,int fatigue=0,int trust=60);
    std::string kind()const override{return "Engineer";}
    std::unique_ptr<Entity> clone()const override;
    void performTurn(GameState&,std::vector<std::string>&)override;
    void addRecord(const std::string& s)override{log_.add(s);}
    const DynamicRecords& records()const override{return log_;}
    DynamicRecords& records()override{return log_;}
    void printDetails(std::ostream& out)const override;
    void repair(GameState& world,std::vector<std::string>& report);
};
class Cryptographer : public Researcher {
    DynamicRecords log_;
public:
    Cryptographer();
    Cryptographer(int id,const std::string& name,int health=100,int fatigue=0,int trust=60);
    std::string kind()const override{return "Cryptographer";}
    std::unique_ptr<Entity> clone()const override;
    void performTurn(GameState&,std::vector<std::string>&)override;
    void addRecord(const std::string& s)override{log_.add(s);}
    const DynamicRecords& records()const override{return log_;}
    DynamicRecords& records()override{return log_;}
    void printDetails(std::ostream& out)const override;
    void decode(GameState& world,std::vector<std::string>& report);
};
class Astrophysicist : public Researcher {
    DynamicRecords log_;
public:
    Astrophysicist();
    Astrophysicist(int id,const std::string& name,int health=100,int fatigue=0,int trust=60);
    std::string kind()const override{return "Astrophysicist";}
    std::unique_ptr<Entity> clone()const override;
    void performTurn(GameState&,std::vector<std::string>&)override;
    void addRecord(const std::string& s)override{log_.add(s);}
    const DynamicRecords& records()const override{return log_;}
    DynamicRecords& records()override{return log_;}
    void printDetails(std::ostream& out)const override;
};
class SecurityOfficer : public Researcher {
    DynamicRecords log_;
public:
    SecurityOfficer();
    SecurityOfficer(int id,const std::string& name,int health=100,int fatigue=0,int trust=60);
    std::string kind()const override{return "SecurityOfficer";}
    std::unique_ptr<Entity> clone()const override;
    void performTurn(GameState&,std::vector<std::string>&)override;
    void addRecord(const std::string& s)override{log_.add(s);}
    const DynamicRecords& records()const override{return log_;}
    DynamicRecords& records()override{return log_;}
    void printDetails(std::ostream& out)const override;
};
class MedicalOfficer : public Researcher {
    DynamicRecords log_;
public:
    MedicalOfficer();
    MedicalOfficer(int id,const std::string& name,int health=100,int fatigue=0,int trust=60);
    std::string kind()const override{return "MedicalOfficer";}
    std::unique_ptr<Entity> clone()const override;
    void performTurn(GameState&,std::vector<std::string>&)override;
    void addRecord(const std::string& s)override{log_.add(s);}
    const DynamicRecords& records()const override{return log_;}
    DynamicRecords& records()override{return log_;}
    void printDetails(std::ostream& out)const override;
};
class CommunicationsSpecialist : public Researcher {
    DynamicRecords log_;
public:
    CommunicationsSpecialist();
    CommunicationsSpecialist(int id,const std::string& name,int health=100,int fatigue=0,int trust=60);
    std::string kind()const override{return "CommunicationsSpecialist";}
    std::unique_ptr<Entity> clone()const override;
    void performTurn(GameState&,std::vector<std::string>&)override;
    void addRecord(const std::string& s)override{log_.add(s);}
    const DynamicRecords& records()const override{return log_;}
    DynamicRecords& records()override{return log_;}
    void printDetails(std::ostream& out)const override;
};
