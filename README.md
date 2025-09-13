
# MessengerCLI - Student Project (Clean, single-folder version)

This project is a self-contained, minimal C++ implementation of a messenger for coursework.
It is intentionally dependency-free (uses only the C++ standard library) so you can build it
without installing SQLite or external crypto libraries.

## Features implemented
- User registration, login, logout, delete account
- Password hashing via `std::hash` (placeholder — **replace** with SHA-256 / bcrypt for real security)
- Save users in `data/users.json` (simple JSON-like format)
- Send messages between users, messages stored in `data/messages.json`
- Chat rooms (create, invite, leave) stored in `data/rooms.json`
- Bot commands: `@echo <text>` and `@all <text>` (interpreted at message send time)
- CLI interactive interface + `run_tests` automated test mode
- Export last N messages of a room to a text file

## Build (Linux/macOS/Windows with mingw)
Using CMake:
```
mkdir build
cd build
cmake ..
cmake --build .
```

Or use g++ directly:
```
g++ -std=c++17 src/*.cpp -o messenger
```

## Run
- Interactive CLI:
```
./messenger
```
- Run automated tests:
```
./messenger run_tests
```

## Notes & Next steps (important for grading)
- **Security**: Current password hashing uses `std::hash` for portability/assignment purposes. Replace `src/Security.cpp` hashing function with a proper SHA-256/Bcrypt/Argon2 implementation (e.g. `picosha2.h` or a crypto library) before submission for real security.
- **Concurrency / multi-client**: This is a single-process CLI application. For multi-client scaling, convert to a client-server architecture (sockets) and use transactional DB (SQLite/MySQL).
- **Improvements**: Use a robust JSON library (nlohmann/json) and SQLite for message storage if permitted by the assignment.

Data files are located under `data/` and example sample users/messages are included.

Good luck — if you want, I can now:
- replace `std::hash` with `picosha2` SHA256 implementation (if you approve),
- convert message storage to SQLite,
- or adapt naming to your existing repository files.
