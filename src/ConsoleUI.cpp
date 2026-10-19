#include "ConsoleUI.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
int ConsoleUI::readInt(const std::string& prompt,int low,int high){
 while(true){std::cout<<prompt;std::string input;if(!std::getline(std::cin,input))throw std::runtime_error("Input closed");std::istringstream stream(input);int number;char extra;
  if(stream>>number && !(stream>>extra) && number>=low && number<=high)return number;
  std::cout<<"Please enter a whole number from "<<low<<" to "<<high<<".\n";
 }
}
std::string ConsoleUI::readLine(const std::string& prompt){std::cout<<prompt;std::string result;if(!std::getline(std::cin,result))throw std::runtime_error("Input closed");return result;}
void ConsoleUI::showMenu()const{
 std::cout<<"\n============ ECHO-7 DIRECTOR TERMINAL ============\n";
 engine_.world().printStatus(std::cout);
 std::cout<<"1 Status  | 2 List  | 3 Inspect  | 4 Create  | 5 Delete  | 6 Duplicate\n"
          <<"7 Modify  | 8 Add record  | 9 Edit record  | 10 Run turns  | 11 Add event\n"
          <<"12 Pending events  | 13 Undo  | 14 Repair special  | 15 Decode special\n"
          <<"16 Evidence  | 17 Investigation  | 18 Observer log  | 19 End / exit\n";
}
void ConsoleUI::createEntity(){
 std::cout<<"1 Engineer, 2 Cryptographer, 3 Astrophysicist, 4 SecurityOfficer, 5 MedicalOfficer, 6 CommunicationsSpecialist\n";
 int type=readInt("Kind: ",1,6);bool defaults=readInt("Use defaults? 1 yes / 2 custom: ",1,2)==1;
 std::string name=readLine("Name (blank = Unnamed): ");if(name.empty())name="Unnamed";
 int h=100,f=0,t=60;if(!defaults){h=readInt("Health: ",0,100);f=readInt("Fatigue: ",0,100);t=readInt("Trust: ",0,100);}
 std::cout<<"Created ID "<<engine_.create(type,name,h,f,t)<<"\n";
}
void ConsoleUI::modifyEntity(){
 int id=readInt("Entity ID: ",1,100000000);
 std::cout<<"1 health 2 fatigue 3 trust 4 active 5 name\n";
 int a=readInt("Attribute: ",1,5);bool ok=false;
 if(a==5)ok=engine_.rename(id,readLine("New name: "));
 else {const char* keys[]={"health","fatigue","trust","active"};int value=readInt("New value: ",0,a==4?1:100);ok=engine_.modify(id,keys[a-1],value);}
 std::cout<<(ok?"Entity updated.\n":"Unknown entity / invalid value.\n");
}
void ConsoleUI::addEvent(){int type=readInt("Type (1 equipment, 2 security, 3 medical, 4 transmission): ",1,4);int id=type==3?readInt("Affected researcher ID: ",1,100000000):0;std::string note=readLine("Event note: ");std::cout<<(engine_.queueManualEvent(type,id,note)?"Queued (will process next turn).\n":"Cannot queue event; check ID.\n");}
void ConsoleUI::specialAction(bool repairing){int id=readInt("Select any entity ID: ",1,100000000);std::vector<std::string> lines;bool ok=repairing?engine_.repairSpecial(id,lines):engine_.decodeSpecial(id,lines);if(!ok){std::cout<<(repairing?"Repair":"Decode")<<" capability not available for this entity, or entity inactive.\n";return;}for(const auto& line:lines)std::cout<<line<<"\n";}
void ConsoleUI::investigate(){
 std::cout<<"Investigation desk (the main story clues unlock across turns):\n";
 std::cout<<"1 Review Roadster cipher  2 Review Experiment 04  3 Review crew discrepancy\n";
 int choice=readInt("Select: ",1,3);
 if(choice==1){std::cout<<"Cipher: WKH ILUVW URDGVWHU. What is the decoded phrase?\n";std::string answer=readLine("Answer: ");if(answer=="THE FIRST ROADSTER"||answer=="the first roadster")std::cout<<"Correct. Compare the 2018 Roadster with the fictional 2028 demonstration.\n";else std::cout<<"Hint: shift each alphabetic character three letters backward.\n";}
 if(choice==2){std::cout<<(engine_.world().turn()>=10?"'The signal didn't predict the outcome. It changed what we decided.'\n":"The archive remains locked until Turn 10.\n");}
 if(choice==3){std::cout<<(engine_.world().turn()>=9?"Dr. Iris Chen — Observer — Experiment 04. Why is she absent from the register?\n":"No confirmed discrepancy has been reported yet.\n");}
}
void ConsoleUI::run(){
 std::cout<<"SIGNAL: THE OBSERVER PARADOX\nECHO-7 // WESTERN DESERT, EGYPT // 2028 (FICTION)\n";
 engine_.initializeScenario();
 while(true){
  try{
   showMenu();int c=readInt("Choose: ",1,19);
   if(c==1)engine_.world().printStatus(std::cout);
   else if(c==2)engine_.world().printEntities(std::cout);
   else if(c==3){int id=readInt("Entity ID: ",1,100000000);const Entity* e=engine_.world().find(id);if(e)e->printDetails(std::cout);else std::cout<<"Unknown ID.\n";}
   else if(c==4)createEntity();
   else if(c==5){int id=readInt("Delete entity ID: ",1,100000000);std::cout<<(engine_.remove(id)?"Deleted.\n":"Unknown ID.\n");}
   else if(c==6){int id=readInt("Duplicate entity ID: ",1,100000000);int newid=engine_.duplicate(id);if(newid<0)std::cout<<"Unknown ID.\n";else std::cout<<"Independent copy created as ID "<<newid<<"\n";}
   else if(c==7)modifyEntity();
   else if(c==8){int id=readInt("Entity ID: ",1,100000000);std::cout<<(engine_.addRecord(id,readLine("New record: "))?"Record saved.\n":"Invalid ID / blank record.\n");}
   else if(c==9){int id=readInt("Entity ID: ",1,100000000);int i=readInt("Record number: ",1,1000000);std::cout<<(engine_.replaceRecord(id,i-1,readLine("Replacement: "))?"Record replaced.\n":"Unknown ID / invalid record.\n");}
   else if(c==10){int turns=readInt("How many turns (1-100): ",1,100);for(const auto& s:engine_.run(turns))std::cout<<s<<"\n";}
   else if(c==11)addEvent();
   else if(c==12)engine_.world().printEvents(std::cout);
   else if(c==13){std::string detail;engine_.undo(detail);std::cout<<detail<<"\n";}
   else if(c==14)specialAction(true);
   else if(c==15)specialAction(false);
   else if(c==16)engine_.world().printEvidence(std::cout);
   else if(c==17)investigate();
   else if(c==18){const auto& m=engine_.observerMemory();if(m.empty())std::cout<<"No erased directives observed.\n";else for(const auto& s:m)std::cout<<"[OBSERVER] "<<s<<"\n";}
   else {std::cout<<"Ending this session.\n";break;}
  }catch(const std::runtime_error& error){std::cout<<"Input ended: "<<error.what()<<"\n";break;}
 }
}
