#include "GameState.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
namespace {int bound(int x){return std::max(0,std::min(100,x));}}
GameState::GameState(const GameState& other):pending_(other.pending_),nextId_(other.nextId_),nextEventId_(other.nextEventId_),turn_(other.turn_),receiver_(other.receiver_),security_(other.security_),research_(other.research_),decoded_(other.decoded_),healthAlerts_(other.healthAlerts_),evidence_(other.evidence_){for(const auto& e:other.entities_)entities_.push_back(e->clone());}
GameState& GameState::operator=(const GameState& other){if(this!=&other){GameState copy(other);entities_.swap(copy.entities_);pending_.swap(copy.pending_);std::swap(nextId_,copy.nextId_);std::swap(nextEventId_,copy.nextEventId_);std::swap(turn_,copy.turn_);std::swap(receiver_,copy.receiver_);std::swap(security_,copy.security_);std::swap(research_,copy.research_);std::swap(decoded_,copy.decoded_);std::swap(healthAlerts_,copy.healthAlerts_);evidence_.swap(copy.evidence_);}return *this;}
int GameState::create(int type,const std::string& name,int h,int f,int t){
 if(type<1||type>6||name.empty())return -1;
 int id=nextId_;
 std::unique_ptr<Entity> item;
 switch(type){
 case 1:item=std::make_unique<Engineer>(id,name,h,f,t);break;
 case 2:item=std::make_unique<Cryptographer>(id,name,h,f,t);break;
 case 3:item=std::make_unique<Astrophysicist>(id,name,h,f,t);break;
 case 4:item=std::make_unique<SecurityOfficer>(id,name,h,f,t);break;
 case 5:item=std::make_unique<MedicalOfficer>(id,name,h,f,t);break;
 case 6:item=std::make_unique<CommunicationsSpecialist>(id,name,h,f,t);break;
 }
 entities_.push_back(std::move(item));++nextId_;return id;
}
int GameState::duplicate(int id){const Entity* original=find(id);if(!original)return -1;auto copy=original->clone();int newId=nextId_;copy->assignId(newId);entities_.push_back(std::move(copy));++nextId_;return newId;}
bool GameState::remove(int id){auto iter=std::find_if(entities_.begin(),entities_.end(),[id](const auto& e){return e->id()==id;});if(iter==entities_.end())return false;entities_.erase(iter);return true;}
Entity* GameState::find(int id){for(auto& e:entities_)if(e->id()==id)return e.get();return nullptr;}
const Entity* GameState::find(int id)const{for(const auto& e:entities_)if(e->id()==id)return e.get();return nullptr;}
bool GameState::modify(int id,const std::string& attr,int value){Entity* e=find(id);return e && e->updateAttribute(attr,value);}
void GameState::addEvidence(const std::string& s){if(std::find(evidence_.begin(),evidence_.end(),s)==evidence_.end())evidence_.push_back(s);}
void GameState::damageReceiver(int x){receiver_=bound(receiver_-x);}
void GameState::repairReceiver(int x){receiver_=bound(receiver_+x);}
void GameState::changeSecurity(int x){security_=bound(security_+x);}
void GameState::changeResearch(int x){research_=std::max(0,research_+x);}
void GameState::queueEvent(EventType type,int targetId,const std::string& note){pending_.enqueue({nextEventId_++,type,targetId,note});}
void GameState::advanceTurn(std::vector<std::string>& report){
 ++turn_;
 report.push_back("============ TURN "+std::to_string(turn_)+" ============");
 // First let every active entity act. Only then process the FIFO queue.
 // We avoid deleting anyone during this loop, so vector iteration stays safe.
 for(auto& e:entities_)if(e->active())e->performTurn(*this,report);
 if(turn_%4==0)queueEvent(EventType::EquipmentAlarm,0,"Receiver temperature drifting");
 if(turn_%6==0)queueEvent(EventType::SecurityIncident,0,"Unexplained archive access");
 while(!pending_.empty()){
     Event ev=pending_.dequeue();
     std::string heading="Event #"+std::to_string(ev.id)+": "+ev.note;
     if(ev.type==EventType::EquipmentAlarm){damageReceiver(12);heading+=" (-12 receiver)";}
     else if(ev.type==EventType::SecurityIncident){changeSecurity(-15);heading+=" (-15 security)";}
     else if(ev.type==EventType::MedicalAlert){Entity* target=find(ev.targetId);auto* person=dynamic_cast<Researcher*>(target);if(person){person->changeHealth(-12);person->changeFatigue(8);heading+=" (medical condition changed)";}else{++healthAlerts_;heading+=" (no eligible researcher; incident archived)";}}
     else {changeResearch(3);heading+=" (+3 research)";}
     report.push_back(heading);
 }
 // Temporary inactive entities remain inspectable. This is deliberate: deleting them
 // automatically would make medical and story consequences harder to investigate.
}
void GameState::printStatus(std::ostream& out)const{out<<"Turn: "<<turn_<<" | Crew/entities: "<<entities_.size()<<" | Receiver: "<<receiver_<<"/100 | Security: "<<security_<<"/100\nResearch: "<<research_<<" | Decoded: "<<decoded_<<" | Pending: "<<pending_.size()<<"\n";}
void GameState::printEntities(std::ostream& out)const{if(entities_.empty()){out<<"No entities in the world.\n";return;}for(const auto& e:entities_)out<<"#"<<e->id()<<" "<<e->name()<<" ["<<e->kind()<<"] "<<(e->active()?"ACTIVE":"INACTIVE")<<"\n";}
void GameState::printEvents(std::ostream& out)const{if(pending_.empty()){out<<"No pending events.\n";return;}for(const Event& e:pending_.values())out<<"#"<<e.id<<" type "<<static_cast<int>(e.type)<<" target "<<e.targetId<<" - "<<e.note<<"\n";}
void GameState::printEvidence(std::ostream& out)const{if(evidence_.empty()){out<<"No evidence found yet.\n";return;}for(size_t i=0;i<evidence_.size();++i)out<<i+1<<". "<<evidence_[i]<<"\n";}
