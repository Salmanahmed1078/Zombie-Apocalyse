# Zombie Apocalypse Game

## Overview

**Zombie Apocalypse** is a console-based C++ game where players navigate a graph-based maze, avoid zombies, collect items, and reach a safe zone. The game offers manual and auto modes, a scoring system, inventory management, and high score tracking. The map is represented as a graph, with nodes connected to form paths, walls, and game elements.

---

## Features

### Game Modes

- **Manual Mode**: Control the player using WASD keys to move through the maze.
- **Auto Mode**: A Breadth-First Search (BFS) algorithm automatically navigates to the safe zone.

### Graph-Based Map

- Nodes represent positions, connected by edges for valid movements.
- Includes walls (`█`), items (`*`), zombies (`Z`), the player (`P`), and the safe zone (`S`).

### Gameplay Mechanics

- Zombies move randomly, deducting 40 health on contact.
- Items:
  - Medkit, Food, 2X multiplier provide health or score bonuses.
- Reach the safe zone to win, or lose if health reaches zero.

### Scoring and Inventory

- **Score Formula**:  
  `Score = (inventory.totalItemValue + distanceTraveled) * 2  -(zombiesEncountered * 10)`
- Inventory tracks collected items.

### High Scores

- Stored in `score.txt`
- Displayed in a sorted leaderboard.

### Console Interface

- ASCII art for home, win, and end screens.
- Animated loading bar and zombie movement on the home screen.
- Color-coded output for walls, items, and UI elements.

---

## Prerequisites

- **Operating System**: Windows (uses `windows.h` for console manipulation)
- **Compiler**: C++11 or later (e.g., MSVC, MinGW)
- **Libraries**:  
  - Standard C++ (`iostream`, `unordered_map`, `vector`, etc.)  
  - `windows.h`, `conio.h`

---

## Installation

```bash
# Clone the repository
git clone <repository-url>

# Or download the ZIP file

# Navigate to the directory
cd zombie-apocalypse-game

# Compile the game
g++ -o zombie_apocalypse Test.cpp

# Run the executable (on Windows)
./zombie_apocalypse
