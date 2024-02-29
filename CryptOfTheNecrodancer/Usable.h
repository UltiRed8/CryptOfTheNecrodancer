#pragma once
#include "Item.h"

enum ListUsable
{
	SHOVEL, PICKAXE, // Shovel
	DAGGER, BROADSWORD, STAFF, BLUNDERBUSS // Weapons
};

class Usable : public Item
{
	int damage;
	ListUsable type;

public :
	void Dig();
	virtual string GetTexturePath() override;
};

