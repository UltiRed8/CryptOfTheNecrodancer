#pragma once
#include "Living.h"
#include "MovementComponent.h"
#include "CollisionComponent.h"	
#include "LifeComponent.h"
#include "DamageComponent.h"
#include "Pickable.h"

class Enemy : public Living
{
protected:
	int indexPatern;
	int currentCooldown;
	int cooldown;
	int droppedCoins;
	vector<Vector2i>directionsPatern;

public:
	Enemy(const float _maxHp,const float _maxDammage,const string& _id, const string& _path, const Vector2f& _position, const int _droppedCoins);
public:
	void DieEvent();
	void InitLifeUI();
	virtual void SelectDirection() = 0;
	virtual void Update() override;
};

