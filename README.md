# Ant Simulation

A real-time ant colony simulation written in C++17 with [SFML 3](https://www.sfml-dev.org/). Virtual ants wander out from a colony, drop pheromone trails, find food, and follow each other's trails back home — producing the kind of emergent foraging behavior real ant colonies exhibit.

## Features

- 🐜 **Agent-based ants** with simple sense-and-steer behavior
- 🌫️ **Pheromone trails** that diffuse and evaporate over time, guiding the colony toward food
- 🏠 **Colony + food sources** rendered live with SFML
- 📊 **Live statistics** plotted from C++ via `matplotlib-cpp` (food collected, ants foraging, etc.)
- ⚙️ **Configurable** simulation parameters in `configuration.cpp` / `include/configuration.h`

## How it works

Each ant is a tiny autonomous agent. It picks a heading, drifts forward, and samples the world around it for two things: food, and pheromones. Two pheromone fields are maintained on a grid:

1. **"To home"** pheromones — dropped by ants leaving the colony.
2. **"To food"** pheromones — dropped by ants returning with food.

Ants searching for food bias their movement toward stronger *to-food* pheromones; ants carrying food do the inverse. Trails evaporate over time, so paths that don't actually lead anywhere fade out, while productive routes get reinforced. No central planner — the swarm-level pathing falls out of these local rules.

## Project layout

```
ant-simulation/
├── include/           # Public headers (ant, colony, world, game, configuration, …)
├── src/               # Implementations
│   ├── main.cpp           # Entry point
│   ├── game.cpp           # Main loop, event handling, rendering
│   ├── world.cpp          # Grid, pheromones, food
│   ├── colony.cpp         # Colony state and spawning
│   ├── ant.cpp            # Per-ant behavior
│   ├── configuration.cpp  # Tunable parameters
│   ├── text.cpp           # On-screen text / HUD
│   └── helper.cpp         # Math + utility helpers
├── media/             # Fonts, textures, etc.
├── CMakeLists.txt     # Build config (auto-fetches SFML 3.0.2)
├── run.sh             # One-shot configure + build + run
└── .clang-format
```

## Build & run

### Prerequisites

- A C++17 compiler (GCC, Clang, or MSVC)
- **CMake ≥ 3.28**
- **Python 3** with development headers (`python3-dev` on Debian/Ubuntu)
- **NumPy** and **matplotlib** (`pip3 install numpy matplotlib`)
- Standard SFML system dependencies (X11, OpenGL, udev, FreeType, etc. on Linux — these are usually pulled in automatically when SFML builds)

> SFML itself doesn't need to be installed system-wide — `CMakeLists.txt` fetches and builds it via `FetchContent`.

### Quick start

```bash
git clone https://github.com/ArterioRodrigues/ant-simulation.git
cd ant-simulation

chmod +x run.sh
./run.sh
```

`run.sh` runs `cmake -B build`, builds the project, and launches `./build/bin/main`.

### Manual build

```bash
cmake -B build
cmake --build build -j
./build/bin/main
```

### Building without matplotlib

If you don't want to install Python / NumPy / matplotlib, you can drop the plotting dependency:

1. Remove the `matplotlib-cpp` include and any plotting calls from the source files that use them.
2. Remove the `find_package(Python3 ...)` block, the NumPy include path, and `Python3::Python` from the `target_link_libraries` call in `CMakeLists.txt`.

The simulation itself only depends on SFML.

## Configuration

Simulation parameters (number of ants, world size, pheromone decay rate, ant speed, sensor angles, etc.) live in `include/configuration.h` and `src/configuration.cpp`. Tweak them, rebuild, and rerun to explore different colony behaviors.

## Development notes

- The CMake build emits `compile_commands.json` (`set(CMAKE_EXPORT_COMPILE_COMMANDS ON)`), so `clangd` and other LSP-based editors will work out of the box. Symlink it to the project root for convenience:
  ```bash
  ln -s build/compile_commands.json
  ```
- A `.clang-format` is included — please format code before submitting changes.

## License

No license file is currently included. Add one (MIT / Apache-2.0 / etc.) before others can reuse the code.
