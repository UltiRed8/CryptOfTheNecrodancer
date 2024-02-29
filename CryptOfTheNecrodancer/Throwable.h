#pragma once
#include "Item.h"

enum ListThrowable
{
	SPEAR, BOMB, THREE_BOMBS
};

class Throwable : public Item
{
	ListThrowable type;

public :
	void Throw(const Vector2f& _distance, const float& _damageDeal);
	virtual string GetTexturePath() override;
};

