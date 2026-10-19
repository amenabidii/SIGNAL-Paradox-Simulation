# SIGNAL: The Observer Paradox

**DRAFT — review, test and improve with both team members.** A C++17 console mystery / turn-based simulation, set in fictional ECHO-7 in Egypt's Western Desert (2028).

The story treats the flying Tesla Roadster as **fiction**. It is not a real-world product announcement. The actual Tesla Roadster launched into space in 2018 is the source of a second interpretation of the signal.

## Compile

```sh
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o signal
./signal
```

Alternatively: `cmake -S . -B build && cmake --build build`.

## Try it

- Start with eight fictional ECHO-7 staff, one mixed `Entity` collection.
- List/inspect/create/duplicate/modify/delete. Add personal history records, then duplicate and edit the copy.
- Run turns, inspect the event queue, manually enqueue a medical, security, equipment or transmission event.
- Ask an arbitrary entity to repair or decode, so unsupported roles demonstrate checked `dynamic_cast`.
- Undo several changes, including simulation turns. The world restores snapshots; the Observer log remains outside the reversible state as a narrative device.
- Investigate the Caesar cipher `WKH ILUVW URDGVWHU`.

## C++ design

`Entity (abstract) -> Researcher (abstract) -> 6 concrete roles`. Every role owns an independent `DynamicRecords` object with its own `new[]` buffer, deep-copy behavior, and destructor. The world owns polymorphic entities through `unique_ptr<Entity>`; copies use virtual `clone()`. Our own `MyQueue` and `MyStack` are linked structures, not banned STL adapters. Normal turns use virtual `performTurn`, while the two special menu actions use checked `dynamic_cast`.

## Important limitations of this draft

The 15 narrative milestones are a lightweight demonstration, **not** the full interactive dialogue/branching game discussed during storytelling. The game's endings consider receiver condition, research and decoded transmissions, rather than a full trust/relationship state machine. The incident chain is simplified. Do not describe it as fully finished without expanding it and testing it yourself.

## AI usage and attribution

This initial draft was generated with ChatGPT after a request for complete source files. Do not state that GitHub Copilot completed or previously developed parts unless that actually happened. Update the AI usage log in `src/main.cpp` to reflect genuine team modifications and further uses.

## Submission note

The assignment asks for a ZIP named `Team_<ID1>_<ID2>.zip` containing **only `.h` and `.cpp` source files**. This GitHub draft ZIP includes documentation, tests and CMake for development. Prepare a separate submission ZIP after the team fills in IDs, verifies AI-disclosure rules, and tests everything.
