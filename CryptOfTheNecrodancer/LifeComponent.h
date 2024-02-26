#pragma once
#include"Component.h"
#include<functional>
#include"TimerManager.h"
using namespace std;
class LifeComponent:public Component
{
	bool invulnerable;
	float* currentHealth;
	float maxHealth;
	function<void()> deathcallback;
	bool isAlive;
	Timer* damageVisualEffect;
	void ResetHealth()
	{
		isAlive = true;
		*currentHealth = maxHealth;
	}
	bool GetIsAlive() const
	{
		return isAlive;
	}
	float* GetCurrentHealth() const
	{
		return currentHealth;
	}
	float GetMaxHealth() const
	{
		return maxHealth;
	}
	
	Timer* GetTimerdamage() const
	{
		return damageVisualEffect;
	}
	
public:
	LifeComponent(Entity* _owner, const function<void()> _deathCallback, const bool _invulnerable = false, const float _maxHealth = 100.0f);
	~LifeComponent();

private:
	void Death();
public:
	virtual void Update() override;
	void ChangeHealth(const float _byAmount);
};

