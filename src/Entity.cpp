#include "Entity.h"
#include "GameState.h"
#include <algorithm>
#include <ostream>
#include <stdexcept>
namespace {int clamp(int x){return std::max(0,std::min(100,x));}}
Entity::Entity():Entity(0,"Unnamed"){}
Entity::Entity(int id,const std::string& name):id_(id),name_(name.empty()?"Unnamed":name){}
void Entity::setName(const std::string& name){if(name.empty())throw std::invalid_argument("Name cannot be empty");name_=name;}
void Entity::assignId(int id){if(id<=0)throw std::invalid_argument("ID must be positive");id_=id;}
bool Entity::updateAttribute(const std::string& key,int value){if(key=="active"&&(value==0||value==1)){setActive(value==1);return true;}return false;}
Researcher::Researcher():Researcher(0,"Unnamed",100,0,60){}
Researcher::Researcher(int id,const std::string& name,int health,int fatigue,int trust):Entity(id,name),health_(clamp(health)),fatigue_(clamp(fatigue)),trust_(clamp(trust)){}
void Researcher::changeHealth(int delta){health_=clamp(health_+delta);if(health_==0)setActive(false);}
void Researcher::changeFatigue(int delta){fatigue_=clamp(fatigue_+delta);}
void Researcher::changeTrust(int delta){trust_=clamp(trust_+delta);}
bool Researcher::updateAttribute(const std::string& key,int value){
 if((key=="health"||key=="fatigue"||key=="trust")&&(value<0||value>100))return false;
 if(key=="health"){health_=value;setActive(value>0);return true;}
 if(key=="fatigue"){fatigue_=value;return true;}
 if(key=="trust"){trust_=value;return true;}
 return Entity::updateAttribute(key,value);
}
void Researcher::printDetails(std::ostream& out)const{out<<"ID: "<<id()<<" | "<<name()<<" | "<<kind()<<"\nHealth: "<<health_<<" | Fatigue: "<<fatigue_<<" | Trust: "<<trust_<<" | Active: "<<(active()?"Yes":"No")<<"\nAssignment: "<<assignment_<<"\nRecords ("<<records().size()<<"):\n";for(int i=0;i<records().size();++i)out<<"  "<<i+1<<". "<<records().at(i)<<"\n";}
void Researcher::finishTurn(const std::string& action,std::vector<std::string>& report)const{report.push_back(name()+" ["+kind()+"]: "+action);}
#define RESEARCHER_CTORS(TYPE) \
 TYPE::TYPE():TYPE(0,"Unnamed"){} \
 TYPE::TYPE(int id,const std::string& name,int h,int f,int t):Researcher(id,name,h,f,t){} \
 std::unique_ptr<Entity> TYPE::clone()const{return std::make_unique<TYPE>(*this);} \
 void TYPE::printDetails(std::ostream& out)const{Researcher::printDetails(out);}
RESEARCHER_CTORS(Engineer)
RESEARCHER_CTORS(Cryptographer)
RESEARCHER_CTORS(Astrophysicist)
RESEARCHER_CTORS(SecurityOfficer)
RESEARCHER_CTORS(MedicalOfficer)
RESEARCHER_CTORS(CommunicationsSpecialist)
#undef RESEARCHER_CTORS
void Engineer::performTurn(GameState& world,std::vector<std::string>& report){
 if(fatigue()>85){changeFatigue(-10);finishTurn("too tired; rested",report);return;}
 world.repairReceiver(2);changeFatigue(5);log_.add("Turn "+std::to_string(world.turn())+": checked receiver");
 finishTurn("stabilized receiver (+2)",report);
}
void Engineer::repair(GameState& world,std::vector<std::string>& report){world.repairReceiver(15);changeFatigue(12);log_.add("Emergency repair on turn "+std::to_string(world.turn()));report.push_back(name()+" performed an emergency repair (+15 receiver).");}
void Cryptographer::performTurn(GameState& world,std::vector<std::string>& report){
 if(fatigue()>85){changeFatigue(-12);finishTurn("rested instead of decoding",report);return;}
 world.changeResearch(4);changeFatigue(7);log_.add("Turn "+std::to_string(world.turn())+": signal analysis");finishTurn("analyzed encrypted traffic (+4 research)",report);
}
void Cryptographer::decode(GameState& world,std::vector<std::string>& report){world.changeResearch(10);world.addDecoded();changeFatigue(14);log_.add("Decoded packet at turn "+std::to_string(world.turn()));report.push_back(name()+" decoded a transmission (+10 research).");}
void Astrophysicist::performTurn(GameState& world,std::vector<std::string>& report){world.changeResearch(3);changeFatigue(4);log_.add("Turn "+std::to_string(world.turn())+": orbital comparison");finishTurn("checked the original 2018 Roadster orbit (+3 research)",report);}
void SecurityOfficer::performTurn(GameState& world,std::vector<std::string>& report){world.changeSecurity(3);changeFatigue(4);log_.add("Turn "+std::to_string(world.turn())+": security sweep");finishTurn("reviewed access logs (+3 security)",report);}
void MedicalOfficer::performTurn(GameState& world,std::vector<std::string>& report){
 for(const auto& entity:world.entities()){auto* person=static_cast<Researcher*>(entity.get());if(person && person->active() && person->fatigue()>65){person->changeFatigue(-7);person->changeHealth(2);}}
 changeFatigue(3);log_.add("Turn "+std::to_string(world.turn())+": medical review");finishTurn("treated fatigued personnel",report);
}
void CommunicationsSpecialist::performTurn(GameState& world,std::vector<std::string>& report){
 changeFatigue(4);log_.add("Turn "+std::to_string(world.turn())+": monitored airwaves");
 if(world.turn()%3==0){world.queueEvent(EventType::Transmission,id(),"Unknown packet from the Roadster frequency");finishTurn("detected a new transmission",report);}
 else finishTurn("monitored the receiver",report);
}
