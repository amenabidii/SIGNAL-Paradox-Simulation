#!/usr/bin/env bash
set -euo pipefail
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude tests/smoke_test.cpp src/DynamicRecords.cpp src/Entity.cpp src/GameState.cpp src/GameEngine.cpp -o build_smoke_test
./build_smoke_test
rm -f build_smoke_test
