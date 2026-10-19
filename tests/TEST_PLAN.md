# Manual test plan — run before submitting

1. Create each of the six kinds with defaults and custom values; reject malformed input.
2. Add three personal records to an Engineer, duplicate, edit duplicate record 2, inspect original: original must be unchanged.
3. Delete original; duplicate still exists; undo; original returns with records and original ID.
4. Undo several edits, deletes and creates in reverse order; empty undo reports cleanly.
5. Run five turns; verify each type produces a distinct report and state changes.
6. Queue Security -> Medical -> Equipment manually. View queue twice; order/length unchanged. Run one turn and confirm event processing in generated order.
7. Try repair with engineer and cryptographer, then decode with cryptographer and engineer. Unsupported casts must not crash.
8. Try nonexistent IDs and invalid numbers, decimals, words and blank lines in all menus.
9. Compile using `g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o signal` and test under AddressSanitizer where available.
10. Run until turn 15 and check the Roadster mystery, Iris Chen and Experiment 04 clues appear.
11. Undo a turn and inspect restored current turn, pending events, records and research.
12. Confirm ending the program cleans all allocated resources.
