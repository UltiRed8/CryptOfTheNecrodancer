#pragma once
#include "Placable.h"
#include "Player.h"

enum PickableType
{
	PT_COIN,PT_DIAMOND,PT_CUSTOM
};

class Pickable : public Placable
{
	PickableType pickableType;
	int amount;
public:

	Pickable(const int _amount, const PickableType& _type, const string& _id,const Vector2f& _pos , const string& _path = "");

public:
	virtual void PickUp();
};

