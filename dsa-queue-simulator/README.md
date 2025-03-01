dsa-queue-simulator
A real-time traffic simulation showcasing queue data structures & traffic management. Includes multiple vehicle types, traffic lights, and priority-based queue handling.

# Prerequisites

You'll need:
- GCC/G++ compiler
- SDL2 library
- MinGW (for Windows)

## How to run

1. Clone the repo:
```bash
git clone https://github.com/bivamshu/dsa-queue-simulator.git
cd DSA-Queue-Simulator
```

2. Compile:
```bash
g++ -Iinclude -Llib -o bin/main.exe src/main.c src/traffic_simulation.c -lmingw32 -lSDL2main -lSDL2
```

```bash
g++ -o bin/generator src/generator.c src/traffic_simulation.c -lSDL2 -Iinclude -Llib -lmingw32 -lSDL2main -lSDL2
```

3. Start the vehicle generator:
```bash
./bin/generator
```
4. Open a new terminal, start the main simulation:
```bash
./bin/main
```