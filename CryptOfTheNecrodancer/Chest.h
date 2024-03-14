#pragma once
#include "Placeable.h"
#include "Item.h"

class Chest : public Placeable
{
public:
	Chest(const Vector2f& _pos);
	~Chest();

public:
	Item* CreateRandomItem();

public:
	void Open();
};

