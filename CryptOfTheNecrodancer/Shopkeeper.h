#pragma once
#include "NPC.h"
#include "MusicManager.h"


#define PATH_SHOPKEEPER "ShopkeeperSprite.png"

class Shopkeeper : public NPC
{
	


public:
	Shopkeeper(const Vector2f& _position);


public:
	void DieEvent();
	void SelectDirection();
};

