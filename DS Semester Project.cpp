#include <iostream>
#include <windows.h> 
#include <cstdlib>   
#include <ctime>     
#include <fstream>   
#include <string> 
#include <iomanip>
#include <thread>  // for sleep_for
#include <chrono>  // for milliseconds
#include <conio.h>
#include"Inventory.h"
#include"ZombieManager.h"
using namespace std;

void moveCursorTo(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}





ZombieManager zombieManager;

// Score & Health
int score = 0;
int health = 100;
int zombiesEncountered = 0;
int distanceTraveled = 0;
string playerName = "";



Inventory inventory;
void clearScreen()
{
    system("cls");
}


void displayScoreAndInventory()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 8);
    int offsetX = 8;
    int offsetY = 3;

    // Final score is calculated based on three factors:
// 1. Total value of collected items (inventory.totalItemValue)
// 2. Total distance the player has traveled (distanceTraveled)
// 3. Number of zombies the player has encountered (zombiesEncountered)

    score = (inventory.totalItemValue + distanceTraveled) * 2
        // The sum of item value and distance is multiplied by 2
        // This rewards both exploration (distance) and item collection equally,
        // giving an extra incentive to move and collect more.

        -(zombiesEncountered * 10);
    // Each zombie encounter deducts 10 points,
    // penalizing players for running into danger.
    // Encourages strategic movement and avoiding zombies when possible.


    moveCursorTo(3 + offsetX, offsetY);
    cout << char(219) << string(107, char(219)) << char(219) << endl;
    moveCursorTo(3 + offsetX, 1 + offsetY);
    cout << char(219);
    moveCursorTo(111 + offsetX, 1 + offsetY);
    cout << char(219);

    moveCursorTo(3 + offsetX, 2 + offsetY);

    cout << char(219);
    SetConsoleTextAttribute(hConsole, 7);
    cout << "       Health: " << health
        << "               |      Score: " << score << "                 |     Inventory: ";
    inventory.display();
    SetConsoleTextAttribute(hConsole, 8);
    moveCursorTo(111 + offsetX, 2 + offsetY);
    cout << char(219);
    cout << endl;
    moveCursorTo(3 + offsetX, 3 + offsetY);
    cout << char(219) << endl;
    moveCursorTo(111 + offsetX, 3 + offsetY);
    cout << char(219);

    moveCursorTo(3 + offsetX, 4 + offsetY);
    cout << char(219) << string(107, char(219)) << char(219) << endl;

    moveCursorTo(3 + offsetX, 5 + offsetY);
    cout << char(219);
    moveCursorTo(111 + offsetX, 5 + offsetY);
    cout << char(219);

    moveCursorTo(3 + offsetX, 6 + offsetY);
    cout << char(219);
    /*  moveCursorTo(110 + offsetX, 6 + offsetY);
      cout << char(219);*/
}



void printMap(node** map, int rows, int cols)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int count = 0;
    char wall = char(219);
    int offsetX = 8;
    int offsetY = 3;

    for (int i = 0; i < rows; i++)
    {
        moveCursorTo(3 + offsetX, 7 + count + offsetY);
        cout << wall;

        moveCursorTo(12 + offsetX, 7 + count + offsetY);
        SetConsoleTextAttribute(hConsole, 7);
        for (int j = 0; j < cols; j++)
        {
            cout << map[i][j].type;
        }
        SetConsoleTextAttribute(hConsole, 8);
        moveCursorTo(111 + offsetX, 7 + count + offsetY);
        cout << wall;

        cout << endl;
        count++;
    }

    int startY = 3 + offsetY;
    int endY = 34 + offsetY;
    int leftX = 3 + offsetX;
    int rightX = 111 + offsetX;

    for (int y = startY; y <= endY; y++) {
        moveCursorTo(leftX, y);
        cout << wall;
        moveCursorTo(rightX, y);
        cout << wall;
    }

    // Bottom wall line
    moveCursorTo(leftX, 35 + offsetY);
    cout << wall << string(107, wall) << wall << endl;

}

void findPlayerPosition(node** map, int rows, int cols, int& playerRow, int& playerCol)
{

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (map[i][j].type == 'P')
            {
                playerRow = i;
                playerCol = j;
                return;
            }
        }
    }
}

void displayWinScreen()
{

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 3);

    moveCursorTo(30, 1);

    cout << R"(                                                                                                  
                                    /==============================================\
                                    || __   _____  _   _  __        _____  _   _  ||
                                    || \ \ / / _ \| | | | \ \      / / _ \| \ | | ||
                                    ||  \ V / | | | | | |  \ \ /\ / / | | |  \| | ||
                                    ||   | || |_| | |_| |   \ V  V /| |_| | |\  | ||
                                    ||   |_| \___/ \___/     \_/\_/  \___/|_| \_| ||
                                    \==============================================/

    )" << endl;
    cin.ignore();


    SetConsoleTextAttribute(hConsole, 10);
    moveCursorTo(30, 15);
    cout << "\t\tWINNING TAKES TALENT,TO REPEAT TAKES CHARACTER " << endl;
    moveCursorTo(30, 17);
    cout << "\t\t\t\tWILL YOU SHOW IT ?" << endl;
    SetConsoleTextAttribute(hConsole, 0);

}

void displayEndScreen()
{

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 3);

    moveCursorTo(30, 1);

    cout << R"(                                                                                                  
        
                  /GGGGGGGG \  AAAAAAAA \  MMMM      MMMM\ EEEEEEE\    /OOOOOOOO \ VVV    VVV\ EEEEEEE\ RRRRRRRR \        !! \
                  GGG    GGG |AAA  __ AAA |MM  MM  MM  MM |EE ____|    OOO    OOO |VVV    VVV |EE ____| RR  __ RR \       !! |
                  GGG         AAA |   AAA |MM \ MMMM / MM |EE|         OOO    OOO |VVV    VVV |EE|      RR |   RRR |      !! |
                  GGG          AAAAAAAAA  |MM |\ MM /  MM |EEEEE\      OOO    OOO |VVV    VVV |EEEEE\   RRRRRRRR<< |      !! |
                  GGG  GGGGG |AAA |   AAA |MM | \__/   MM |EE|         OOO    OOO |VVVVVVVVVV |EE|      RR |    RR |      !! |
                  GGG    GGG |AAA |   AAA |MM |        MM |EE|         OOO    OOO |  VVVVVV  / EE|      RR |     RR \     --- 
                  GGGGGGGG / AAA |    AAA |MM |        MM |EEEEEEE\    \$$$$$$$$ /    VVVV  /  EEEEEEE\ RR |      RR \    !! |
                  \_______/  \___|   \___|\__|        \__|\______|     \_______/     \____|   \______| \__|      \___|   ___/
                  =============================================================================================================


    )" << endl;
    cin.ignore();


    SetConsoleTextAttribute(hConsole, 10);
    moveCursorTo(30, 15);
    cout << "\t\t\tYOU CAN NOT GIVE UP JUST YET ..... " << endl;
    moveCursorTo(30, 17);
    cout << "\t\t\t\tTRY AGAIN.. ?" << endl;
    SetConsoleTextAttribute(hConsole, 0);

}


bool movePlayer(node** map, int rows, int cols, char direction)
{
    int playerRow, playerCol;
    findPlayerPosition(map, rows, cols, playerRow, playerCol);

    int newRow = playerRow;
    int newCol = playerCol;

    // Player can only move on actual game rows (even rows)
    switch (direction)
    {
    case 'w':
        // Move up two rows when going up (skip the spacing row)
        newRow--;
        break;
    case 's':
        // Move down two rows when going down (skip the spacing row)
        newRow++;
        break;
    case 'a':
        newCol--;
        break;
    case 'd':
        newCol++;
        break;
    default:
        return false;
    }

    // Check for out of bounds
    if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols)
    {
        return false;
    }

    // Make sure we're landing on a game row (even row), not a spacing row
   /* if (newRow % 2 != 0)
    {
        return false;
    }*/

    // Check for wall collision - using ASCII 219 for walls
    if (map[newRow][newCol].type == char(219))
    {
        return false;
    }

    // Handle zombie encounter
    if (map[newRow][newCol].type == 'Z')
    {
        health -= 40;
        zombiesEncountered++;
        zombieManager.remove(newRow, newCol);
    }
    // Handle item/food
    else if (map[newRow][newCol].type == '*')
    {
        int itemType = rand() % 3;
        if (itemType == 0)
        {
            inventory.addItem("Medkit", 3);
            health += 50;
        }
        else if (itemType == 1)
        {
            inventory.addItem("Food", 2);
            health += 20;
        }
        else
        {
            inventory.addItem("2X", 2);
            score = (score * 2);
        }
    }
    // Handle exit point
    else if (map[newRow][newCol].type == 'S')
    {
        map[playerRow][playerCol].type = '-';
        map[newRow][newCol].type = 'P';
        distanceTraveled++;

        clearScreen();
        displayWinScreen();

        if (!playerName.empty())
        {
            ofstream scoreFile("score.txt", ios::app);
            if (scoreFile.is_open()) {
                scoreFile << playerName << ": " << score << endl;
                scoreFile.close();
            }
        }

        exit(0);
    }

    // Move player
    map[playerRow][playerCol].type = '-';
    map[newRow][newCol].type = 'P';
    distanceTraveled++;
    return true;
}

void initializeMap(node** map, int rows, int cols)
{
    // Initialize all nodes with proper connections
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            map[i][j].row = i;
            map[i][j].col = j;
            map[i][j].type = '-';  // Default to empty space
        }
    }

    // Set up node connections
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i > 0)
                map[i][j].up = &map[i - 1][j];
            if (i < rows - 1)
                map[i][j].down = &map[i + 1][j];
            if (j > 0)
                map[i][j].left = &map[i][j - 1];
            if (j < cols - 1)
                map[i][j].right = &map[i][j + 1];
        }
    }

    // Create outer border walls for all rows
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1)
            {
                map[i][j].type = char(219);  // ASCII 219 for walls
            }
        }
    }

    // Create maze walls - now only horizontal walls on game rows and vertical walls on all rows

    // Horizontal wall segments - ONLY on game rows (even rows)
    // First area - top left
    for (int j = 5; j < 20; j++)
    {
        if (j != 10)
        {
            map[4][j].type = char(219); // Game row only
        }
    }

    for (int j = 10; j < 25; j++)
    {
        if (j != 20)
        {
            map[6][j].type = char(219); // Game row only
        }
    }

    // Second area - top right
    for (int j = 30; j < 45; j++)
    {
        if (j != 35)
        {
            map[8][j].type = char(219); // Game row only
        }
    }

    for (int j = 50; j < 70; j++)
    {
        if (j != 60)
        {
            map[12][j].type = char(219); // Game row only
        }
    }

    // Third area - bottom left
    for (int j = 5; j < 30; j++)
    {
        if (j != 15)
        {
            map[16][j].type = char(219); // Game row only
        }
    }

    // Fourth area - bottom right
    for (int j = 45; j < 85; j++)
    {
        if (j != 55 && j != 70)
        {
            map[18][j].type = char(219); // Game row only
        }
    }

    // Additional horizontal segments
    for (int j = 25; j < 40; j++)
    {
        if (j != 32)
        {
            map[10][j].type = char(219); // Game row only
        }
    }

    for (int j = 15; j < 35; j++)
    {
        if (j != 25)
        {
            map[14][j].type = char(219); // Game row only
        }
    }

    for (int j = 65; j < 80; j++)
    {
        if (j != 72)
        {
            map[4][j].type = char(219); // Game row only
        }
    }

    // Vertical wall segments - on BOTH game and spacing rows
    // First area - left side
    for (int i = 0; i < rows; i++)
    {
        if (i >= 2 && i <= 8 && i != 4)
        {
            map[i][10].type = char(219); // Both game and spacing rows
        }
    }

    for (int i = 0; i < rows; i++)
    {
        if (i >= 4 && i <= 10 && i != 8)
        {
            map[i][20].type = char(219); // Both game and spacing rows
        }
    }

    // Second area - middle
    for (int i = 0; i < rows; i++)
    {
        if (i >= 2 && i <= 14 && i != 8)
        {
            map[i][35].type = char(219); // Both game and spacing rows
        }
    }

    for (int i = 0; i < rows; i++)
    {
        if (i >= 10 && i <= 18 && i != 14)
        {
            map[i][50].type = char(219); // Both game and spacing rows
        }
    }

    // Third area - right side
    for (int i = 0; i < rows; i++)
    {
        if (i >= 2 && i <= 10 && i != 6)
        {
            map[i][60].type = char(219); // Both game and spacing rows
        }
    }

    for (int i = 0; i < rows; i++)
    {
        if (i >= 12 && i <= 18 && i != 16)
        {
            map[i][70].type = char(219); // Both game and spacing rows
        }
    }

    // Additional vertical segments
    for (int i = 0; i < rows; i++)
    {
        if (i >= 8 && i <= 16 && i != 12)
        {
            map[i][25].type = char(219); // Both game and spacing rows
        }
    }

    for (int i = 0; i < rows; i++)
    {
        if (i >= 4 && i <= 12 && i != 8)
        {
            map[i][45].type = char(219); // Both game and spacing rows
        }
    }

    for (int i = 0; i < rows; i++)
    {
        if (i >= 14 && i <= 20 && i != 18)
        {
            map[i][55].type = char(219); // Both game and spacing rows
        }
    }

    for (int i = 0; i < rows; i++)
    {
        if (i >= 2 && i <= 8 && i != 4)
        {
            map[i][75].type = char(219); // Both game and spacing rows
        }
    }


    // Small maze sections - only on game rows
    // Top right maze

    map[2][65].type = char(219);
    map[2][66].type = char(219);
    map[2][67].type = char(219);
    map[4][65].type = char(219);
    map[4][67].type = char(219);

    // Middle maze section
    map[10][30].type = char(219);
    map[10][31].type = char(219);
    map[10][32].type = char(219);
    map[10][33].type = char(219);
    map[12][30].type = char(219);
    map[12][31].type = char(219);
    map[12][32].type = char(219);
    map[12][33].type = char(219);
    map[16][1].type = char(219);
    map[16][2].type = char(219);
    map[16][3].type = char(219);
    map[16][4].type = char(219);
    map[16][5].type = char(219);
    // Bottom maze section
    map[19][40].type = char(219);
    map[19][41].type = char(219);
    map[19][42].type = char(219);
    map[20][10].type = char(219);
    map[20][11].type = char(219);
    map[21][10].type = char(219);
    map[22][11].type = char(219);
    map[8][1].type = char(219);

    // Add vertical walls in specific places to make corridors
    for (int i = 4; i < 10; i++) {
        map[i][15].type = char(219);
    }

    for (int i = 8; i < 16; i++) {
        map[i][40].type = char(219);
    }

    for (int i = 2; i < 12; i++) {
        map[i][80].type = char(219);
    }

    for (int i = 10; i < 20; i++) {
        map[i][65].type = char(219);
    }


    moveCursorTo(20, 10);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 2); // Dark Yellow
    // Add items (food/power-ups) - only on game rows (even rows) and NOT on border walls
    map[2][2].type = '*';      // Safe - inside border
    map[2][15].type = '*';     // Safe - inside border
    map[4][8].type = '*';      // Safe - inside border
    map[6][28].type = '*';     // Safe - inside border
    map[10][40].type = '*';    // Safe - inside border
    map[14][55].type = '*';    // Safe - inside border
    map[8][70].type = '*';     // Safe - inside border
    map[18][30].type = '*';    // Safe - inside border
    map[2][15].type = '*';     // Safe - inside border
    map[20][55].type = '*';    // FIXED - moved from border to inside (row 20, col 55)
    map[4][42].type = '*';     // Safe - inside border
    map[16][75].type = '*';    // Safe - inside border
	map[21][40].type = '*';    // Safe - inside border
	map[20][60].type = '*';    // Safe - inside border
	map[22][80].type = '*';    // Safe - inside border
    SetConsoleTextAttribute(hConsole, 7); // Dark Yellow
    // Set exit point - on a game row (NOT on border)
    map[rows - 2][cols - 3].type = 'S';  // FIXED - moved from border edge to one space in

    // Set player's starting position - on a game row (NOT on border)
    map[2][2].type = 'P';  // FIXED - moved from border (0,2) to inside (2,2)
    SetConsoleTextAttribute(hConsole, 9); // Dark Yellow
    // Add zombies - only on game rows
    zombieManager.initZombie(map, 2, 15);
    zombieManager.initZombie(map, 4, 25);
    zombieManager.initZombie(map, 10, 50);
    zombieManager.initZombie(map, 14, 30);
    zombieManager.initZombie(map, 6, 60);
    zombieManager.initZombie(map, 12, 15);
    zombieManager.initZombie(map, 8, 80);
    zombieManager.initZombie(map, 16, 5);

    // Lower part (rows 18–23)
    zombieManager.initZombie(map, 20, 20);
    zombieManager.initZombie(map, 21, 15);
    zombieManager.initZombie(map, 15, 6);
    zombieManager.initZombie(map, 20, 80);

    // Middle part (rows 10–17)
    zombieManager.initZombie(map, 12, 20);
    zombieManager.initZombie(map, 15, 40);

    // Upper part (rows 1–9)
    zombieManager.initZombie(map, 3, 70);
    zombieManager.initZombie(map, 5, 90);

}
class BFSPathfinder
{
private:
    struct Point
    {
        int row, col;
    };

    static const int MAX_QUEUE = 1000;
    Point bfsQueue[MAX_QUEUE];
    int qFront, qRear;

    void enqueue(Point p)
    {
        if ((qRear + 1) % MAX_QUEUE != qFront)
        {
            bfsQueue[qRear] = p;
            qRear = (qRear + 1) % MAX_QUEUE;
        }
    }

    Point dequeue()
    {
        Point p = bfsQueue[qFront];
        qFront = (qFront + 1) % MAX_QUEUE;
        return p;
    }

    bool isQueueEmpty() const
    {
        return qFront == qRear;
    }

    void resetQueue()
    {
        qFront = qRear = 0;
    }

public:
    BFSPathfinder() : qFront(0), qRear(0)
    {
        for (int i = 0; i < MAX_QUEUE; i++)
        {
            bfsQueue[i] = { 0,0 };
        }
    }

    bool findPathToSafeZone(node** map, int rows, int cols, Point path[], int& pathLength)
    {
        static bool visited[50][50] = {};
        static Point parent[50][50];
        bool found = false;

        int playerRow, playerCol;
        findPlayerPosition(map, rows, cols, playerRow, playerCol);

        resetQueue();
        enqueue({ playerRow, playerCol });
        visited[playerRow][playerCol] = true;

        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };

        Point dest;

        while (!isQueueEmpty())
        {
            Point current = dequeue();

            if (map[current.row][current.col].type == 'S')
            {
                dest = current;
                found = true;
                break;
            }

            for (int d = 0; d < 4; d++)
            {
                int nr = current.row + dr[d];
                int nc = current.col + dc[d];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                    !visited[nr][nc] && (map[nr][nc].type == '-' || map[nr][nc].type == 'S' || map[nr][nc].type == '*'))
                {
                    visited[nr][nc] = true;
                    enqueue({ nr, nc });
                    parent[nr][nc] = current;
                }
            }
        }

        if (!found)
        {
            return false;
        }

        pathLength = 0;
        while (!(dest.row == playerRow && dest.col == playerCol))
        {
            path[pathLength++] = dest;
            dest = parent[dest.row][dest.col];
        }

        return true;
    }

    void runAutoMode(node** map, int rows, int cols)
    {
        Point path[500];
        int pathLength;
        int moveCounter = 0;

        if (!findPathToSafeZone(map, rows, cols, path, pathLength))
        {
            cout << "No path to safe zone!\n";
            system("pause");
            return;
        }

        for (int i = pathLength - 1; i >= 0; i--)
        {
            int playerRow, playerCol;
            findPlayerPosition(map, rows, cols, playerRow, playerCol);

            map[playerRow][playerCol].type = '-';

            int newRow = path[i].row;
            int newCol = path[i].col;

            char targetCell = map[newRow][newCol].type;

            if (targetCell == 'Z')
            {
                health -= 40;
                zombiesEncountered++;
                zombieManager.remove(newRow, newCol);
            }
            else if (targetCell == '*')
            {
                int itemType = rand() % 2;
                if (itemType == 0) {
                    inventory.addItem("Medkit", 3);
                    health += 50;
                }
                else
                {
                    inventory.addItem("Food", 2);
                    health += 20;
                }
            }

            map[newRow][newCol].type = 'P';
            distanceTraveled++;
            moveCounter++;

            clearScreen();
            displayScoreAndInventory();

            printMap(map, rows, cols);

            if (targetCell == 'S')
            {
                clearScreen();
                displayWinScreen();
                system("pause");
                return;
            }

            zombieManager.spawnZombieIfNeeded(map, rows, cols, moveCounter);
            zombieManager.processZombies(map, rows, cols);

            if (health <= 0)
            {
                clearScreen();
                displayEndScreen();
                system("pause");
                return;
            }
            Sleep(500);
        }

        int playerRow, playerCol;
        findPlayerPosition(map, rows, cols, playerRow, playerCol);
        if (map[playerRow][playerCol].type == 'S')
        {
            clearScreen();
            displayScoreAndInventory();
            printMap(map, rows, cols);
            displayWinScreen();
        }
        else
        {
            clearScreen();
            displayScoreAndInventory();
            printMap(map, rows, cols);
            displayEndScreen();
        }

        system("pause");
    }
};

void displayHighScore() {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 3);

    moveCursorTo(30, 1);

    cout << R"(                                                                                                  
                               
                                            /============================================================\
                                            ||    _   _ ___ ____ _   _     ____   ____ ___  ____  _____   ||
                                            ||   | | | |_ _/ ___| | | |   / ___| / ___/ _ \|  _ \| ____|  ||
                                            ||   | |_| || | |  _| |_| |   \___ \| |  | | | | |_) |  _|    ||
                                            ||   |  _  || | |_| |  _  |    ___) | |__| |_| |  _ <| |___   ||
                                            ||   |_| |_|___\____|_| |_|   |____/ \____\___/|_| \_\_____|  ||
                                            \============================================================/  
             )" << endl;
    cin.ignore();
}


void HighScores()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const int size = 100;
    string names[size];
    int scores[size];
    int count = 0;

    ifstream scoreFile("score.txt");
    if (!scoreFile.is_open())
    {
        cout << "No high scores available yet.\n";
        return;
    }

    while (count < size && getline(scoreFile, names[count], ':'))
    {
        string scoreStr;
        if (getline(scoreFile, scoreStr))
        {
            int score = 0;
            for (int i = 0; i < scoreStr.length(); i++)
            {
                if (scoreStr[i] >= '0' && scoreStr[i] <= '9')
                {
                    score = score * 10 + (scoreStr[i] - '0');
                }
            }
            scores[count] = score;
            count++;
        }
    }
    scoreFile.close();

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                swap(scores[j], scores[j + 1]);
                swap(names[j], names[j + 1]);
            }
        }
    }
    SetConsoleTextAttribute(hConsole, 3); // Blue color
    moveCursorTo(47, 12);
    cout << string(45, char(219)); // Top border

    moveCursorTo(47, 13);
    cout << char(219) << string(43, ' ') << char(219); // Padding line

    moveCursorTo(47, 14);
    cout << char(219);
    SetConsoleTextAttribute(hConsole, 6); // Blue color
    cout << "     ~*~*~*~*~*~HIGH SCORES~*~*~*~*~*~     ";  // Title centered
    SetConsoleTextAttribute(hConsole, 3); // Blue color
    cout << char(219);
    moveCursorTo(47, 15);
    cout << char(219) << string(43, ' ') << char(219); // Padding line

    moveCursorTo(47, 16);
    cout << string(30, char(219)); // Bottom border




    int limit = count < 10 ? count : 10;

    for (int i = 0; i < limit; i++)
    {
        SetConsoleTextAttribute(hConsole, 3); // Blue color
        moveCursorTo(47, 16 + i);
        cout << char(219) << string(43, ' ') << char(219); // Empty row with borders

        moveCursorTo(49, 16 + i);
        SetConsoleTextAttribute(hConsole, 11); // Blue color
        string displayName = names[i];
        if (displayName.length() > 15)
            displayName = displayName.substr(0, 15);

        cout << setw(7) << right << (i + 1) << ". "
            << left << setw(20) << displayName
            << ": " << setw(5) << right << scores[i];
    }
    SetConsoleTextAttribute(hConsole, 3); // Blue color
    // Bottom border
    moveCursorTo(47, 16 + limit);
    cout << char(219) << string(43, ' ') << char(219); // Empty row with borders
    moveCursorTo(47, 17 + limit);
    cout << string(45, char(219));

    SetConsoleTextAttribute(hConsole, 7); // Reset to white
}




//// Cursor mover for positioning text
//void moveCursorTo(int x, int y) {
//    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
//    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
//}
void clearZombie(int x, int yStart, int height = 12) {
    for (int i = 0; i < height; ++i) {
        moveCursorTo(x, yStart + i);
        cout << string(30, ' ');
    }
}


void showHomeScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");

    // Zombie art
    string zombie[] = {
    string("     (") + char(219) + char(219) + char(219) + char(219) + char(219) + char(219) + ")          ",
    "    /        \\        ",
    string("   |  ") + char(219) + "  " + char(219) + "   |        ",
    string("   |   ") + char(220) + char(220) + "    |        ",
    string("   |  ") + char(219) + char(219) + char(219) + char(219) + "   |        ",
    string("    \\") + char(219) + char(219) + char(219) + char(219) + char(219) + char(219) + "/          ",
    string("   /|  ") + char(219) + char(219) + " |\\          ",
    string("  /_|__") + char(219) + char(219) + "__|_\\         ",
    string("    ") + char(219) + char(219) + "  " + char(219) + char(219) + "            ",
    "   / |  | \\           ",
    "  /__|  |__\\          ",
    string("     ") + char(219) + char(219) + "  " + char(219) + char(219) + "           "
    };


    const int FRAME_TOP = 10;     // Shifted further down
    const int FRAME_LEFT = 18;   // Shifted further right
    const int FRAME_WIDTH = 100;
    const int FRAME_HEIGHT = 18;


    // Draw top border
    SetConsoleTextAttribute(hConsole, 6); // Dark Yellow
    moveCursorTo(FRAME_LEFT, FRAME_TOP);
    cout << string(FRAME_WIDTH, char(220));

    // Side borders
    for (int i = 1; i < FRAME_HEIGHT - 1; ++i) {
        moveCursorTo(FRAME_LEFT, FRAME_TOP + i);
        cout << string(2, char(219)) << string(FRAME_WIDTH - 4, ' ') << string(2, char(219));
    }

    // Bottom border
    moveCursorTo(FRAME_LEFT, FRAME_TOP + FRAME_HEIGHT - 2);
    cout << string(FRAME_WIDTH, char(220));

    // Add eerie decoration
    moveCursorTo(FRAME_LEFT + 2, FRAME_TOP + 1);
    SetConsoleTextAttribute(hConsole, 4);
    cout << "  ~ BLOOD ~";

    moveCursorTo(FRAME_LEFT + FRAME_WIDTH - 20, FRAME_TOP + 1);
    SetConsoleTextAttribute(hConsole, 12);
    cout << "~DANGER ZONE ~";

    // Walk zombie inside the frame
    int zombieY = FRAME_TOP + 2;
    int zombieStartX = FRAME_LEFT + 2;
    int zombieEndX = FRAME_LEFT + FRAME_WIDTH - 35;

    for (int x = zombieStartX; x < zombieEndX; ++x) {
        SetConsoleTextAttribute(hConsole, 2); // Green zombie

        for (int i = 0; i < 12; ++i) {
            moveCursorTo(x, zombieY + i);
            cout << zombie[i];
        }

        this_thread::sleep_for(chrono::milliseconds(40));

        for (int i = 0; i < 12; ++i) {
            moveCursorTo(x, zombieY + i);
            cout << string(zombie[i].length(), ' ');
        }
    }
    // Pause before showing title
    this_thread::sleep_for(chrono::milliseconds(500));
    system("cls");

    // Apocalypse title
    string apocalypseTitle[] = {
     R"(        ||                        /ZZZZZZZ |/OOOOOOOO \ MMMM      MMMM\ BBBBBBBB\  IIIIII\EEEEEEE\                             ||)",
     R"(        ||                             ZZ / OOO    OOO |MM  MM  MM  MM |BB  __ BB\ \_II _|EE ____|                             ||)",
     R"(        ||                            ZZ /  OOO    OOO |MM \ MMMM / MM |BB |   BBB|  II | EE|                                  ||)",
     R"(        ||                           ZZ /   OOO    OOO |MM |\ MM /  MM |BBBBBBB<< |  II | EEEEE\                               ||)",
     R"(        ||                          ZZ /    OOO    OOO |MM | \__/   MM |BB  __ BBB|  II | EE|                                  ||)",
     R"(        ||                         ZZ /__   OOO    OOO |MM |        MM |BB |   BB |  II | EE|                                  ||)",
     R"(        ||                        ZZZZZZZZ\ \OOOOOOOO / MM |        MM |BBBBBBBB / IIIIII\EEEEEEE\                             ||)",
     R"(        ||                        \_______|  \_______/  \__|        \__|\_______|  \_____|\______|                             ||)",
     R"(        ||                                                                                                                     ||)",
     R"(        ||        AAAAAAAA \  PPPPPPPP\  /OOOOOOOO \ /CCCCCCCC \  AAAAAAAA \  LL\    YY      YY\ SSSSSS\   EEEEEEE\    !! \    ||)",
     R"(        ||       AAA  __ AAA |PP  __ PP\ OOO    OOO |CCC    CCC |AAA  __ AAA |LL |   YY      YY |SS  __SS\ EE ____|    !! |    ||)",
     R"(        ||       AAA |   AAA |PP |   PPP|OOO    OOO |CCC  ______|AAA |   AAA |LL |   YY      YY |SS /  \__|EE|         !! |    ||)",
     R"(        ||        AAAAAAAA<< |PPPPPPP<</ OOO    OOO |CCC |        AAAAAAAA<< |LL |   YY      YY |\SSSSSS\  EEEEE\      !! |    ||)",
     R"(        ||       AAA |   AAA |PP  ____/  OOO    OOO |CCC |       AAA |   AAA |LL |   YYYYYYYYYY | \____SS\ EE|         !! |    ||)",
     R"(        ||       AAA |   AAA |PP |       OOO    OOO |CCC    CCC |AAA |   AAA |LL +--     YY ____|SS\   SS |EE|         ---     ||)",
     R"(        ||       AAA |   AAA |PP |       \OOOOOOOO / CCCCCCCCC / AAA |   AAA |LLLLLL\    YY |    \SSSSSS  |EEEEEEE\    !! |    ||)",
     R"(        ||       \___|   \___|\__|        \_______/  \________/  \___|   \___|\______|   \__|     \______/ \______|    ___/    ||)"
    };

    int colors[] = { 4, 12, 6, 14, 10, 11, 13, 5 };

    for (int i = 0; i < 6; ++i) {
        SetConsoleTextAttribute(hConsole, colors[i % 8]);
        moveCursorTo(0, 8);

        for (auto line : apocalypseTitle) {
            // Replace every "||" with two char(219)
            size_t pos = 0;
            while ((pos = line.find("||", pos)) != string::npos) {
                line.replace(pos, 2, string(2, char(219)));
                pos += 2;
            }
            cout << line << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
    }
}






void showLoadingBar() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int left = 40, top = 12;
    int frameWidth = 80;
    int barWidth = 60;

    // Draw frame using ASCII codes
    moveCursorTo(left, top);
    SetConsoleTextAttribute(hConsole, 15); // White text
    cout << char(201) << string(frameWidth, char(205)) << char(187); // Top border

    for (int i = 1; i <= 8; ++i) {
        moveCursorTo(left, top + i);
        cout << char(186) << string(frameWidth, ' ') << char(186); // Side borders
    }

    moveCursorTo(left, top + 9);
    cout << char(200) << string(frameWidth, char(205)) << char(188); // Bottom border

    // Loading text
    moveCursorTo(left + 23, top + 2);
    SetConsoleTextAttribute(hConsole, 14); // Yellow
    cout << "Loading... Please wait";

    // Loading bar brackets
    moveCursorTo(left + 5, top + 5);
    SetConsoleTextAttribute(hConsole, 10); // Green
    cout << "[";
    moveCursorTo(left + 5 + barWidth + 1, top + 5);
    cout << "]";

    // Loading animation
    for (int i = 0; i <= barWidth; ++i) {
        moveCursorTo(left + 6 + i, top + 5);
        cout << char(219); // Block character

        // Show percentage
        int percentage = (i * 100) / barWidth;
        moveCursorTo(left + 25, top + 6);
        SetConsoleTextAttribute(hConsole, 11); // Cyan
        cout << "Progress: " << percentage << "%   ";

        Sleep(30);
    }

    // Final message
    moveCursorTo(left + 24, top + 8);
    SetConsoleTextAttribute(hConsole, 10); // Green
    cout << "Loading Complete!";

    // Reset to white
    SetConsoleTextAttribute(hConsole, 15);

}


#define RESET_COLOR 7
#define BOX_COLOR 3        // Gray/dark
#define TITLE_COLOR 14      // Yellow
#define TEXT_COLOR 11  


void displayCommands() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    const int boxWidth = 45;
    const int startX = 123;
    const int startY = 15;
    const char BOX_CHAR = (char)219;

    string commands[] = {
        "W   ---->   Fly North",
        "A   ---->   Run West",
        "S   ---->   Dig South",
        "D   ---->   Dash East",
        "Q   ---->   Retreat to safety"
    };

    // Top Border
    SetConsoleTextAttribute(hConsole, BOX_COLOR);
    moveCursorTo(startX, startY);
    for (int i = 0; i < boxWidth; ++i) cout << BOX_CHAR;

    // Padding above title
    moveCursorTo(startX, startY + 1);
    cout << BOX_CHAR << string(boxWidth - 2, ' ') << BOX_CHAR;

    // Title
    moveCursorTo(startX, startY + 2);
    cout << BOX_CHAR;
    SetConsoleTextAttribute(hConsole, TITLE_COLOR);
    cout << " ~*~*~*~*~*~ Movement Commands ~*~*~*~*~*~";
    SetConsoleTextAttribute(hConsole, BOX_COLOR);
    cout << string(boxWidth - 1 - 43, ' ') << BOX_CHAR;

    // Padding below title
    moveCursorTo(startX, startY + 3);
    cout << BOX_CHAR << string(boxWidth - 2, ' ') << BOX_CHAR;

    // Command lines
    for (int i = 0; i < 5; ++i) {
        moveCursorTo(startX, startY + 4 + i);
        SetConsoleTextAttribute(hConsole, BOX_COLOR);
        cout << BOX_CHAR;
        SetConsoleTextAttribute(hConsole, TEXT_COLOR);
        cout << "     " << commands[i];
        SetConsoleTextAttribute(hConsole, BOX_COLOR);
        cout << string(boxWidth - 2 - 5 - commands[i].length(), ' ') << BOX_CHAR;
    }

    // Bottom padding
    moveCursorTo(startX, startY + 9);
    cout << BOX_CHAR << string(boxWidth - 2, ' ') << BOX_CHAR;

    // Bottom Border
    moveCursorTo(startX, startY + 10);
    for (int i = 0; i < boxWidth; ++i) cout << BOX_CHAR;

    // Reset color
    SetConsoleTextAttribute(hConsole, RESET_COLOR);
}

void drawMainMenu(int selected) {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // === TITLE ===
    SetConsoleTextAttribute(hConsole, 11); // Cyan for title
    moveCursorTo(35, 1);
    cout << R"(                                                                                                  
                             MMM       MMMM\    AAAAAAAA \   IIIIII\ NNNN     NN\    MMMM     MMMM\ EEEEEEE\ NNNN     NN\ UUU    UUU\ 
                             MM  MM  MM  MM |  AAA  __ AAA | \_II _| NN NN    NN |   MM  MM  MM MM |EE ____| NN NN    NN |UUU    UUU |
                             MM \ MMMM / MM |  AAA |   AAA |   II |  NN  NN   NN |   MM |\ MMMM MM |EE|      NN  NN   NN |UUU    UUU | 
                             MM |\ MM /  MM |   AAAAAAAA   |   II |  NN | NN  NN |   MM | \ MM /MM |EEEEE\   NN | NN  NN |UUU    UUU |
                             MM | \__/   MM |  AAA |   AAA |   II |  NN |\ NN NN |   MM |  \__/ MM |EE __|   NN |\ NN NN |UUU    UUU |
                             MM |        MM |  AAA |   AAA |   II |  NN | \ NNNN |   MM |       MM |EE|      NN | \ NNNN |UUU    UUU |
                             MM |        MM |  AAA |   AAA | IIIIII\ NN |  \ NNN |   MM |       MM |EEEEEEE\ NN |  \ NNN |\UUUUUUUU / 
                             \__|        \__|  \___|   \___| \_____| \__|   \____|   \__|       \__|\______| \__|   \____| \_______/  
                            ===========================================================================================================
    )";

    // === MENU BOX ===
    int menuTop = 22;
    int menuLeft = 50;
    int boxWidth = 55;
    int lineSpacing = 2;
    string options[] = {
        "Manual Mode",
        "Start Auto Mode",
        "View High Score",
        "Exit"
    };
    int totalOptions = sizeof(options) / sizeof(options[0]);

    int totalHeight = totalOptions * lineSpacing + 2; // space between + padding

    // Top Border
    SetConsoleTextAttribute(hConsole, 11); // Cyan for the top border
    moveCursorTo(menuLeft, menuTop);
    cout << char(219) << string(boxWidth - 2, char(219)) << char(219);

    // Options with spacing
    for (int i = 0; i < totalOptions; ++i) {
        int y = menuTop + 1 + i * lineSpacing;

        // Empty padding line (above each option)
        moveCursorTo(menuLeft, y);
        cout << char(219) << string(boxWidth - 2, ' ') << char(219);

        // Option line
        moveCursorTo(menuLeft, y + 1);

        // Set the side borders (left and right) to cyan for each line
        SetConsoleTextAttribute(hConsole, 11); // Cyan for left and right borders
        cout << char(219); // Left border

        // Set the color for the menu options text (white by default, orange when selected)
        if (i == selected)
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); // Orange background with white tex        }
        else {
            SetConsoleTextAttribute(hConsole, 15); // White for default menu options
        }

        // Print the option content (move it to the right)
        string content = "             ";// Adjust this padding to move the content right
        content += options[i];
        int pad = boxWidth - 2 - content.length();
        cout << content << string(pad, ' ');

        // Print the right side border in cyan
        SetConsoleTextAttribute(hConsole, 11); // Cyan for right border
        cout << char(219); // Right border
    }

    // Bottom padding line
    moveCursorTo(menuLeft, menuTop + totalHeight - 1);
    SetConsoleTextAttribute(hConsole, 11); // Cyan for the side and bottom border
    cout << char(219) << string(boxWidth - 2, ' ') << char(219);

    // Bottom Border
    moveCursorTo(menuLeft, menuTop + totalHeight);
    cout << char(219) << string(boxWidth - 2, char(219)) << char(219);

    SetConsoleTextAttribute(hConsole, 7); // Reset to default console color
}



int showMainMenu() {
    int selected = 0;
    drawMainMenu(selected);

    while (true) {
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72 && selected > 0) { // Arrow Up
                selected--;
                drawMainMenu(selected);
            }
            else if (key == 80 && selected < 3) { // Arrow Down
                selected++;
                drawMainMenu(selected);
            }
        }
        else if (key == 13) { // Enter
            return selected;
        }
    }
}




int main() {
    showHomeScreen();
     showLoadingBar();

    srand(static_cast<unsigned int>(time(0)));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int rows = 24, cols = 92;
    node** map = new node * [rows];
    for (int i = 0; i < rows; i++) {
        map[i] = new node[cols];
    }

    BFSPathfinder pathfinder;

    while (true) {
        clearScreen();
        // displayMainMenu();

        int choice = showMainMenu(); // 0=Manual, 1=Auto, 2=Score, 3=Exit

        if (choice == 3) {
            cout << "Exited" << endl;
            break;
        }

        if (choice == 2) {
            clearScreen();
            displayHighScore();
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 6);
            HighScores();
            SetConsoleTextAttribute(hConsole, 7);
            system("pause");
            continue;
        }

        initializeMap(map, rows, cols);
        health = 100;
        score = zombiesEncountered = distanceTraveled = 0;
        inventory = Inventory();

        if (choice == 0) {
            system("cls"); // Clear screen
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 11); // Cyan for title
            moveCursorTo(35, 2); // Adjust the position of the title
            // === Title (Name Profile in ASCII Art) ===
            cout <<
                R"(
                                                    /======================================================================\
                                                    ||    ____  _                         ____             __ _ _         ||
                                                    ||   |  _ \| | __ _ _   _  ___ _ __  |  _ \ _ __ ___  / _(_) | ___    ||
                                                    ||   | |_) | |/ _` | | | |/ _ \ '__| | |_) | '__/ _ \| |_| | |/ _ \   ||
                                                    ||   |  __/| | (_| | |_| |  __/ |    |  __/| | | (_) |  _| | |  __/   ||
                                                    ||   |_|   |_|\__,_|\__, |\___|_|    |_|   |_|  \___/|_| |_|_|\___|   ||
                                                    ||                  |___/                                             ||
                                                    \======================================================================/                       
)";

            // === Cyan Outer Box ===
            int left = 58;              // Outer cyan box left position
            int top = 15;               // Outer cyan box top position
            int boxWidth = 60;          // Width of cyan box
            int boxHeight = 11;         // Height of cyan box
            int inputBoxHeight = 5;     // Height of white input are
            // === Cyan Outer Box (Thick Borders) ===
            SetConsoleTextAttribute(hConsole, 11);
            for (int i = 0; i < boxHeight; ++i) {
                moveCursorTo(left, top + i);
                if (i == 0 || i == boxHeight - 1) {
                    // Top and bottom thick border
                    cout << string(boxWidth, char(219));
                }
                else {
                    // Side borders thick
                    cout << string(2, char(219)) << string(boxWidth - 4, ' ') << string(2, char(219));
                }
            }

            // === Prompt Text ===
            SetConsoleTextAttribute(hConsole, 3); // Orange/light red
            moveCursorTo(left + 4, top + 2);
            cout << "ENTER YOUR NAME :";

            // === White Input Box Dimensions ===
            int inputLeft = left + 4;
            int inputTop = top + 4;
            int inputWidth = boxWidth - 8;

            // === White Input Box Drawing ===
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
            for (int i = 0; i < inputBoxHeight; ++i) {
                moveCursorTo(inputLeft, inputTop + i);
                cout << string(inputWidth, ' ');
            }

            // === Input Cursor Location & Style ===
            SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE); // Black on white
            moveCursorTo(inputLeft, inputTop);
            getline(cin, playerName);

            // === Confirm Entry ===
            SetConsoleTextAttribute(hConsole, 7); // Neutral
            moveCursorTo(40, top + boxHeight + 2);
            cout << "Name Profile Updated: " << playerName;

            char input;
            int playerMoveCounter = 0;

            while (true) {
                clearScreen();
                displayScoreAndInventory();
                printMap(map, rows, cols);
                displayCommands();

                moveCursorTo(130, 38);
                cin >> input;

                SetConsoleTextAttribute(hConsole, 7);
                input = tolower(input);

                if (input == 'q') {
                    break;
                }

                if (!movePlayer(map, rows, cols, input)) {
                    cout << "Invalid move! Try again.\n";
                    Sleep(500);
                }
                else {
                    playerMoveCounter++;
                }

                zombieManager.spawnZombieIfNeeded(map, rows, cols, playerMoveCounter);
                zombieManager.processZombies(map, rows, cols);

                if (health <= 0) {
                    clearScreen();
                    displayEndScreen();
                    system("pause");

                    if (!playerName.empty()) {
                        ofstream scoreFile("score.txt", ios::app);
                        if (scoreFile.is_open()) {
                            scoreFile << playerName << ": " << score << endl;
                            scoreFile.close();
                        }
                    }
                    break;
                }
            }
        }
        else if (choice == 1) {
            pathfinder.runAutoMode(map, rows, cols);
        }
    }

    for (int i = 0; i < rows; i++) {
        delete[] map[i];
    }
    delete[] map;

    return 0;
}
