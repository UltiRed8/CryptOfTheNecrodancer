#pragma once
#include"Enemy.h"
#include "MusicManager.h"


#define PATH_SHOPKEEPER "ShopkeeperSprite.png"

class Shopkeeper : public Enemy
{
	


public:
	Shopkeeper(const Vector2f& _position);


public:
	void DieEvent();
	void SelectDirection();
	void UpdateRythm();

};

