#pragma once
#include "Items.h"

class Throwable : public Items
{

public :
	void Throw(const Vector2f& _distance, const float& _damageDeal);
};

