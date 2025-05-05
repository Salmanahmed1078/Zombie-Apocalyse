Zombie Apocalypse Game
Overview
Zombie Apocalypse is a console-based C++ game where players navigate a graph-based maze, avoid zombies, collect items, and reach a safe zone. The game offers manual and auto modes, a scoring system, inventory management, and high score tracking. The map is represented as a graph, with nodes connected to form paths, walls, and game elements.
Features

Game Modes:
Manual Mode: Control the player using WASD keys to move through the maze.
Auto Mode: A Breadth-First Search (BFS) algorithm automatically navigates to the safe zone.


Graph-Based Map:
Nodes represent positions, connected by edges for valid movements.
Includes walls (█), items (*), zombies (Z), the player (P), and the safe zone (S).


Gameplay Mechanics:
Zombies move randomly, deducting 40 health on contact.
Items (Medkit, Food, 2X multiplier) provide health or score bonuses.
Reach the safe zone to win or lose if health reaches zero.


Scoring and Inventory:
Score = (Item value × 10) - (Zombies encountered × 5) + (Distance traveled × 2).
Inventory tracks collected items.


High Scores:
Stored in score.txt and displayed in a sorted leaderboard.


Console Interface:
ASCII art for home, win, and end screens.
Animated loading bar and zombie movement on the home screen.
Color-coded output for walls, items, and UI elements.



Prerequisites

Operating System: Windows (uses windows.h for console manipulation).
Compiler: C++11 or later (e.g., MSVC, MinGW).
Libraries: Standard C++ libraries (iostream, unordered_map, vector, etc.), windows.h, conio.h.

Installation

Clone or Download:git clone <repository-url>

Or download the source code as a ZIP file.
Navigate to Directory:cd zombie-apocalypse-game


Compile:g++ -o zombie_apocalypse Test.cpp

Ensure a Windows-compatible compiler is used.
Run:./zombie_apocalypse



Usage

Start the Game:
Run the executable to see the animated home screen and loading bar.


Main Menu:
Navigate using Up/Down arrow keys and select with Enter:
Manual Mode: Play manually.
Start Auto Mode: BFS-driven gameplay.
View High Score: See top 10 scores.
Exit: Quit the game.




Manual Mode:
Enter your player name.
Controls:
W: Move up
A: Move left
S: Move down
D: Move right
Q: Return to main menu


Avoid zombies, collect items, and reach the safe zone.


Auto Mode:
Watch the BFS algorithm navigate the player to the safe zone, updating every 500ms.


Game Outcomes:
Win: Reach the safe zone (S) to see the win screen and save your score.
Lose: Health reaches zero, showing the end screen and saving your score.


High Scores:
View the leaderboard from the main menu, sorted by score.



Game Elements

Player (P): Starts at (2,2), controlled by the player or BFS.
Zombies (Z): Move randomly, spawn after 7 player moves, deduct 40 health.
Items (*):
Medkit: +50 health, +3 item value.
Food: +20 health, +2 item value.
2X: Doubles score, +2 item value.


Safe Zone (S): Exit point near the bottom-right.
Walls (█): Impassable obstacles forming the maze.
