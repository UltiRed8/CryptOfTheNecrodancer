#pragma once
#include "Placeable.h"
#include "Item.h"

class Chest : public Placeable
{
	vector<int> ownedItems;

public:
	Chest(const Vector2f& _pos);

public:
	Item* CreateRandomItem();

private:
	void UpdateOwnedItems();

public:
	void Open();
};