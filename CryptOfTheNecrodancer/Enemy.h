#pragma once
#include"Entity.h"
#include "MovementComponent.h"
#include "CollisionComponent.h"	
#include "LifeComponent.h"
#include "DamageComponent.h"


class Enemy:public Entity
{
protected:
	int indexPatern;
	int currentCooldown;
	int cooldown;
	vector<Vector2i>directionsPatern;

public:
	Enemy(const string& _id, const string& _path, const Vector2f& _position);
	void InitAllBattlesComponents(const float _damagesAmounts,const float _maxHealth);
public:
	void DieEvent();
};

