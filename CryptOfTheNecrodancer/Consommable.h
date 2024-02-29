#pragma once
#include "Item.h"

enum ListConsommable
{
	APPLE, CHEESE, DRUMSTICK, HAM, CARROT, COOKIES
};

class Consommable : public Item
{
	ListConsommable type;

public :
	void Heal(float _healAmount);
	virtual string GetTexturePath() override;
};

