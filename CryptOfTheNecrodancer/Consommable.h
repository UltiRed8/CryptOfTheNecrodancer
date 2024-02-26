#pragma once
#include "Items.h"

class Consommable : public Items
{
	int count;

public :
	Consommable();

public :
	void Heal(float _healAmount);
};

