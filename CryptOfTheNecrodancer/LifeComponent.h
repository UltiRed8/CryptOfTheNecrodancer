#pragma once

#include"Component.h"
#include"TimerManager.h"
#include"ProgressBar.h"

#include<functional>

using namespace std;

class LifeComponent:public Component
{
	bool invulnerable;
	float* currentHealth;
	float maxHealth;
	function<void()> deathcallback;
	bool isAlive;
	ProgressBar* healthBar;
	Vector2f pBPos;

public :
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
	ProgressBar* GetHealthBar() const
	{
		return healthBar;
	}
	
public:
	LifeComponent(Entity* _owner, const function<void()> _deathCallback, const bool _invulnerable = false, const float _maxHealth = 100.0f, bool _isPlayer = false);
	~LifeComponent();

private:
	void Death();

public:
	virtual void Update() override;
	bool ChangeHealth(const float _byAmount);
};