#pragma once
#include <iostream>
#include "Menu.h"
#include "Item.h"

using namespace std;

class Inventory
{
	Menu* playerInventory;
	vector<Item*> items;

public :
	Inventory();
	~Inventory();

public :
	void AddItem(Item* _item);
	void RemoveItem(Item* _item);
};

