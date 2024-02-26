#include "LifeComponent.h"
#include"Entity.h"
LifeComponent::LifeComponent(Entity* _owner, const function<void()> _deathCallback, const bool _invulnerable, const float _maxHealth):Component(_owner)
{
	invulnerable = _invulnerable;
	maxHealth = _maxHealth;
	currentHealth = new float(maxHealth);
	deathcallback = _deathCallback;
	isAlive = true;
	damageVisualEffect = new Timer(owner->GetID() + "DamageVisualEffect", [this]() { owner->GetShape()->setFillColor(Color::White); }, seconds(0.25f), 0);

}

LifeComponent::~LifeComponent()
{
	delete currentHealth;
	damageVisualEffect->Pause();
	damageVisualEffect->Destroy();
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
	*currentHealth += _byAmount;
	if (_byAmount < 0)
	{
		owner->GetShape()->setFillColor(Color::Red);
		
		damageVisualEffect->Run(1);
	}
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
