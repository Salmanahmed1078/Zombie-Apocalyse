#include < iostream>
using namespace std; 


struct node
{
    int row, col;
    char type;
    node* up;
    node* down;
    node* left;
    node* right;
    node() : row(0), col(0), type('-'), up(NULL), down(NULL), left(NULL), right(NULL) {}
};

struct Zombie
{
    int row;
    int col;

    Zombie() : row(0), col(0) {}
    Zombie(int r, int c) : row(r), col(c) {}
};

class ZombieManager
{
    static const int MAX_ZOMBIES = 100;
    Zombie zombieQueue[MAX_ZOMBIES];
    int front, rear;

public:
    ZombieManager() : front(0), rear(0)
    {
        for (int i = 0; i < MAX_ZOMBIES; i++)
        {
            zombieQueue[i] = { 0,0 };
        }
    }

    bool isEmpty()
    {
        return front == rear;
    }

    bool isFull()
    {
        return (rear + 1) % MAX_ZOMBIES == front;
    }

    void enqueue(Zombie z)
    {
        if (!isFull())
        {
            zombieQueue[rear] = z;
            rear = (rear + 1) % MAX_ZOMBIES;
        }
    }

    Zombie dequeue()
    {
        Zombie z = zombieQueue[front];
        front = (front + 1) % MAX_ZOMBIES;
        return z;
    }

    void remove(int row, int col)
    {
        int newFront = front;
        int newRear = front;

        while (newFront != rear)
        {
            Zombie z = zombieQueue[newFront];
            if (!(z.row == row && z.col == col))
            {
                zombieQueue[newRear] = z;
                newRear = (newRear + 1) % MAX_ZOMBIES;
            }
            newFront = (newFront + 1) % MAX_ZOMBIES;
        }
        rear = newRear;
    }

    int size()
    {
        return (rear - front + MAX_ZOMBIES) % MAX_ZOMBIES;
    }

    void processZombies(node** map, int rows, int cols)
    {
        int count = size();
        for (int i = 0; i < count; i++)
        {
            Zombie z = dequeue();
            int r = z.row, c = z.col;

            if (map[r][c].type == 'Z')
            {
                map[r][c].type = '-';
            }

            int dir = rand() % 4;
            switch (dir)
            {
            case 0:
                if (r > 0 && map[r - 1][c].type == '-')
                {
                    r--;
                }
                break;

            case 1:
                if (r < rows - 1 && map[r + 1][c].type == '-')
                {
                    r++;
                }
                break;

            case 2:
                if (c > 0 && map[r][c - 1].type == '-')
                {
                    c--;
                }
                break;

            case 3:
                if (c < cols - 1 && map[r][c + 1].type == '-')
                {
                    c++;
                }
                break;
            }
            map[r][c].type = 'Z';
            enqueue({ r, c });
        }
    }

    void spawnZombieIfNeeded(node** map, int rows, int cols, int& moveCounter)
    {
        if (moveCounter >= 7)
        {
            int r = rand() % rows;
            int c = rand() % cols;

            while (map[r][c].type != '-' || r == 0 || r == rows - 1 || c == 0 || c == cols - 1)
            {
                r = rand() % rows;
                c = rand() % cols;
            }
            map[r][c].type = 'Z';
            enqueue({ r, c });
            moveCounter = 0;
        }
    }

    void initZombie(node** map, int r, int c)
    {
        map[r][c].type = 'Z';
        enqueue({ r, c });
    }
};
