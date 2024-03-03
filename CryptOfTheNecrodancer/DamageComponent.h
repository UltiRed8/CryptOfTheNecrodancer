#pragma once
#include"Component.h"
using namespace std;

class DamageComponent:public Component
{
	float damagesAmount;
	bool canAttack;
public:
	
	float GetDamagesAmount() const
	{
		return damagesAmount;
	}
	
public:
	DamageComponent(Entity* _owner, const float _damagesAmount,  const bool _canAttack = true);
	

public:
	bool Attack(Entity* _target);
	virtual void Update() override;
};

