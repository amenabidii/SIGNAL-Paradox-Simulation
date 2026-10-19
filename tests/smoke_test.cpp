#include "GameEngine.h"
#include <cassert>
#include <sstream>
#include <iostream>

int main() {
    GameEngine game;
    int original = game.create(1, "Tester", 82, 3, 45);
    assert(original > 0);
    assert(game.addRecord(original, "One"));
    assert(game.addRecord(original, "Two"));
    int copy = game.duplicate(original);
    assert(copy > original);
    assert(game.replaceRecord(copy, 0, "Changed"));
    assert(game.world().find(original)->records().at(0) == "One");
    assert(game.remove(original));
    assert(game.world().find(original)==nullptr);
    std::string note;
    assert(game.undo(note));
    assert(game.world().find(original)->records().at(0)=="One");
    assert(game.undo(note)); // record edit
    assert(game.world().find(copy)->records().at(0)=="One");
    assert(game.undo(note)); // duplication
    assert(game.world().find(copy)==nullptr);
    assert(game.undo(note)); // added record Two
    assert(game.world().find(original)->records().size()==1);
    assert(game.undo(note)); // added record One
    assert(game.world().find(original)->records().size()==0);
    assert(game.undo(note)); // create
    assert(game.world().find(original)==nullptr);
    assert(!game.undo(note));

    MyQueue<int> queue;
    queue.enqueue(3);queue.enqueue(4);
    MyQueue<int> clonedQueue=queue;
    assert(queue.dequeue()==3 && queue.dequeue()==4 && queue.empty());
    assert(clonedQueue.dequeue()==3 && clonedQueue.dequeue()==4);

    MyStack<int> stack;
    stack.push(5);stack.push(9);
    MyStack<int> clonedStack=stack;
    assert(stack.pop()==9 && stack.pop()==5);
    assert(clonedStack.pop()==9 && clonedStack.pop()==5);

    game.initializeScenario();
    assert(game.world().entities().size()==8);
    std::vector<std::string> report;
    assert(game.repairSpecial(1,report));
    assert(!game.decodeSpecial(1,report));
    assert(game.decodeSpecial(2,report));
    assert(!game.repairSpecial(2,report));
    assert(game.queueManualEvent(2,0,"first"));
    assert(game.queueManualEvent(1,0,"second"));
    auto events = game.world().pending().values();
    assert(events.size()==2 && events[0].note=="first" && events[1].note=="second");
    game.run(3);
    assert(game.world().turn()==3);
    std::cout<<"SIGNAL smoke tests passed.\n";
}
