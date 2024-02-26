#pragma once
#include "Items.h"

enum ListUsable
{
	SHOVEL, PICKAXE, // Shovel
	DAGGER, BROADSWORD, STAFF, BLUNDERBUSS // Weapons
};

class Usable : public Items
{
	int damage;
	ListUsable type;

public :
	void Dig();
	virtual string GetTexturePath() override;
};

