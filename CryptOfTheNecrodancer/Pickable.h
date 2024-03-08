#pragma once
#include "Placeable.h"
#include "Player.h"

enum PickableType
{
	PT_COIN,PT_DIAMOND,PT_CUSTOM
};

class Pickable : public Placeable
{
	PickableType pickableType;
	Timer* animationTimer;
	float animationValue;
	int amount;

public:

	Pickable(const int _amount, const PickableType& _type, const string& _id,const Vector2f& _pos , const string& _path = "");
public:
	virtual void PickUp();
};


