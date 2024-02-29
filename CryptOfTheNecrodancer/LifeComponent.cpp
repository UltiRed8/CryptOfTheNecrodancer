#include "LifeComponent.h"
#include"Entity.h"
LifeComponent::LifeComponent(Entity* _owner, const function<void()> _deathCallback, const bool _invulnerable, const float _maxHealth):Component(_owner)
{
	invulnerable = _invulnerable;
	maxHealth = _maxHealth;
	currentHealth = new float(maxHealth);
	deathcallback = _deathCallback;
	isAlive = true;
}

LifeComponent::~LifeComponent()
{
	delete currentHealth;
}

void LifeComponent::Death()
{
	isAlive = false;
	if (deathcallback)
	{
		deathcallback();
	}
}

void LifeComponent::Update()
{
}

void LifeComponent::ChangeHealth(const float _byAmount)
{
	if (invulnerable) return;
	cout << *currentHealth << endl;
	*currentHealth += _byAmount;
	cout << *currentHealth << endl;
	if (*currentHealth <= 0.0f)
	{
		*currentHealth = 0.0f;

		Death();
	}
	else if (*currentHealth > maxHealth)
	{
		*currentHealth = maxHealth;
	}
	
}
