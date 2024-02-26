#pragma once
#include "Items.h"

enum ListConsommable
{
	APPLE, CHEESE, DRUMSTICK, HAM, CARROT, COOKIES
};

class Consommable : public Items
{
	ListConsommable type;

public :
	void Heal(float _healAmount);
	virtual string GetTexturePath() override;
};

