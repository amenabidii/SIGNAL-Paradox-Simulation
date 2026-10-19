#include "GameEngine.h"
#include <algorithm>
#include <stdexcept>
void GameEngine::remember(const std::string& action){decisionHistory_.push_back(action);}
int GameEngine::create(int type,const std::string& name,int h,int f,int t){if(type<1||type>6||name.empty())return -1;undoHistory_.push(world_);int id=world_.create(type,name,h,f,t);remember("Created #"+std::to_string(id)+" "+name);return id;}
int GameEngine::duplicate(int id){if(!world_.find(id))return -1;undoHistory_.push(world_);int newId=world_.duplicate(id);remember("Duplicated #"+std::to_string(id)+" as #"+std::to_string(newId));return newId;}
bool GameEngine::remove(int id){if(!world_.find(id))return false;undoHistory_.push(world_);world_.remove(id);remember("Removed #"+std::to_string(id));return true;}
bool GameEngine::modify(int id,const std::string& key,int value){if(!world_.find(id))return false;GameState snapshot(world_);if(!world_.modify(id,key,value))return false;undoHistory_.push(snapshot);remember("Modified #"+std::to_string(id)+" "+key);return true;}
bool GameEngine::rename(int id,const std::string& name){Entity* e=world_.find(id);if(!e||name.empty())return false;GameState snapshot(world_);e->setName(name);undoHistory_.push(snapshot);remember("Renamed #"+std::to_string(id));return true;}
bool GameEngine::addRecord(int id,const std::string& entry){Entity* e=world_.find(id);if(!e||entry.empty())return false;GameState snapshot(world_);e->addRecord(entry);undoHistory_.push(snapshot);remember("Added personal record to #"+std::to_string(id));return true;}
bool GameEngine::replaceRecord(int id,int index,const std::string& entry){Entity* e=world_.find(id);if(!e||index<0||index>=e->records().size()||entry.empty())return false;GameState snapshot(world_);e->records().replace(index,entry);undoHistory_.push(snapshot);remember("Edited private record of #"+std::to_string(id));return true;}
bool GameEngine::queueManualEvent(int kind,int targetId,const std::string& text){if(kind<1||kind>4|| (kind==3&&!world_.find(targetId)))return false;undoHistory_.push(world_);world_.queueEvent(static_cast<EventType>(kind),targetId,text.empty()?"Director-created incident":text);remember("Queued event type "+std::to_string(kind));return true;}
bool GameEngine::repairSpecial(int id,std::vector<std::string>& report){Entity* e=world_.find(id);auto* engineer=dynamic_cast<Engineer*>(e);if(!engineer||!engineer->active())return false;undoHistory_.push(world_);engineer->repair(world_,report);remember("Requested specialized repair from #"+std::to_string(id));return true;}
bool GameEngine::decodeSpecial(int id,std::vector<std::string>& report){Entity* e=world_.find(id);auto* cryptographer=dynamic_cast<Cryptographer*>(e);if(!cryptographer||!cryptographer->active())return false;undoHistory_.push(world_);cryptographer->decode(world_,report);remember("Requested specialized decoding from #"+std::to_string(id));return true;}
bool GameEngine::undo(std::string& detail){if(undoHistory_.empty()){detail="Nothing left to undo.";return false;}world_=undoHistory_.pop();detail="Last world change restored (including records and pending events).";if(!decisionHistory_.empty()){observerMemory_.push_back(decisionHistory_.back());detail+=" The Observer may remember.";decisionHistory_.pop_back();}return true;}
void GameEngine::initializeScenario(){
 if(started_)return;
 started_=true;
 const char* names[]={"Noah Reed","Elias Vale","Mara Voss","Daniel Cross","Lena Ortiz","Olivia Bennett","Victor Hale","Adrian Keller"};
 int types[]={1,2,3,4,5,6,3,1};
 for(int i=0;i<8;++i)world_.create(types[i],names[i],95,8,65);
 world_.addEvidence("ECHO-7, Western Desert, Egypt. Official crew count: eight.");
}
void GameEngine::applyStory(std::vector<std::string>& report){
 const int turn=world_.turn();
 if(turn==1){report.push_back("[EARTH NEWS] FICTIONAL 2028: Tesla's flying Roadster demonstration begins near Giza.");report.push_back("[SIGNAL 001] IT WAS NOT SUPPOSED TO FLY.");world_.addEvidence("Transmission 001: IT WAS NOT SUPPOSED TO FLY. Source unknown.");}
 if(turn==2){report.push_back("[ENCRYPTED] WKH ILUVW URDGVWHU (Caesar shift -3)");world_.addEvidence("A cipher fragment mentions THE FIRST ROADSTER (possible 2018 space launch).");}
 if(turn==3){report.push_back("[REED] The receiver's calibration was modified. Its condition will matter.");}
 if(turn==4){report.push_back("[ORTIZ] Vale needs rest. Check personnel fatigue before assigning additional work.");}
 if(turn==5){report.push_back("[EARTH NEWS] The Roadster temporarily vanished from tracking. A signal timestamp matches.");world_.addEvidence("Roadster tracking interruption matches earlier signal metadata; causation unproven.");}
 if(turn==6){report.push_back("[CROSS] Someone used Voss's credentials in the restricted archive. She denies it.");if(!observerMemory_.empty())report.push_back("[UNKNOWN] YOU CHANGED YOUR MIND. I REMEMBER.");}
 if(turn==7){report.push_back("[RECEIVER] An anomaly in the calibration is causing additional heating.");world_.damageReceiver(14);}
 if(turn==8){report.push_back("[SECURITY] Eight registered. Nine counted by the occupancy sensors.");world_.addEvidence("Occupancy discrepancy: eight registered, nine detected.");}
 if(turn==9){report.push_back("[ARCHIVE] A photograph shows a ninth person: Dr. Iris Chen, OBSERVER.");world_.addEvidence("Archived Experiment 04 personnel record: Dr. Iris Chen, Observer.");}
 if(turn==10){report.push_back("[EXPERIMENT 04] 'The signal didn't predict the outcome. It changed what we decided.'");world_.addEvidence("Experiment 04 investigated how warnings influenced researcher decisions.");}
 if(turn==11){report.push_back("[ROADSTER RECORD] The same message appears in an old record linked to the 2018 Roadster.");world_.addEvidence("The Roadster phrase predates the fictional 2028 demonstration.");}
 if(turn==12){report.push_back("[VOSS] 'I remember Iris standing behind your chair. This morning.'");}
 if(turn==13){report.push_back("[SIGNAL 009] AT 04:00 THE RECEIVER WILL FAIL. ONE OF YOU WILL NOT LEAVE. DO NOT DISCONNECT.");world_.queueEvent(EventType::EquipmentAlarm,0,"Predicted receiver strain");}
 if(turn==14){report.push_back("[IRIS CHEN - ARCHIVE] 'Someone must observe without responding.'");world_.addEvidence("Iris Chen questioned the assumption that the signal was giving instructions.");}
 if(turn==15){report.push_back("[SIGNAL] THIS IS WHERE YOU ALWAYS CHOOSE.");report.push_back("[REGISTRY] ID 000: DIRECTOR / OBSERVING. 'YOU CANNOT SEARCH FOR YOURSELF.'");}
}
std::vector<std::string> GameEngine::run(int count){
 std::vector<std::string> report;
 if(count<1||count>100)return {"Invalid turn count (1-100)."};
 for(int i=0;i<count;++i){undoHistory_.push(world_);remember("Advanced simulation to turn "+std::to_string(world_.turn()+1));world_.advanceTurn(report);applyStory(report);if(world_.turn()>=15)report.push_back("[FINAL STATUS] "+ending());}
 return report;
}
std::string GameEngine::ending()const{
 if(world_.turn()<15)return "Investigation in progress.";
 if(world_.receiver()<45)return "THE SILENCE: The receiver fails. ECHO-7 prioritizes evacuating the crew.";
 if(world_.decoded()>=2 && world_.research()>=90)return "THE OBSERVATION: Experiment 05 concluded. Preparing Experiment 06...";
 return "THE PARADOX: The team ignores the warning and follows evidence. 'THAT IS WHAT SHE THOUGHT.'";
}
