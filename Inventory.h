#include<iostream>
using namespace std; 

struct InventoryNode
{
    string itemName;
    int itemValue;
    InventoryNode* next;
    InventoryNode(string name, int value) : itemName(name), itemValue(value), next(NULL) {}
};

class Inventory
{
public:
    InventoryNode* head;
    InventoryNode* tail;
    int totalItemValue;

    Inventory() : head(NULL), tail(NULL), totalItemValue(0) {}

    void addItem(string name, int value)
    {
        InventoryNode* newItem = new InventoryNode(name, value);

        if (!head)
        {
            head = newItem;
            tail = newItem;
        }
        else
        {
            tail->next = newItem;
            tail = newItem;
        }

        totalItemValue += value;
    }

    void display()
    {
        InventoryNode* curr = head;
        cout << "[";

        while (curr)
        {
            cout << curr->itemName;

            if (curr->next)
            {
                cout << ", ";
            }

            curr = curr->next;
        }

        cout << "]";
    }
};