#include "LifeComponent.h"
#include"Entity.h"
#include"Player.h"
#define EMPTY "UI/HealthBarEmpty.png"
#define FULL "UI/HealthBarFull.png"

LifeComponent::LifeComponent(Entity* _owner, const function<void()> _deathCallback, const bool _invulnerable, const float _maxHealth, bool _isPlayer) : Component(_owner)
{
	invulnerable = _invulnerable;
	maxHealth = _maxHealth;
	currentHealth = new float(maxHealth);
	deathcallback = _deathCallback;
	isAlive = true;
	if (_isPlayer) return;
	pBPos = Vector2f(_owner->GetPosition().x, _owner->GetPosition().y - TILE_SIZE.y / 2.f);
	healthBar = new ProgressBar(STRING_ID("LifeEnemy"), PT_LEFT, pBPos, Vector2f(50.0f, 10.0f),
		EMPTY, FULL, currentHealth);
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
	pBPos = Vector2f(owner->GetPosition().x, owner->GetPosition().y - TILE_SIZE.y / 2.f);
	healthBar->SetPosition(pBPos);
}

bool LifeComponent::ChangeHealth(const float _byAmount)
{
	if (invulnerable) return false;
	*currentHealth += _byAmount;
	if (owner->GetType() != ET_PLAYER)
	{
		healthBar->SetValue(*currentHealth);
	}
	if (*currentHealth <= 0.0f)
	{
		*currentHealth = 0.0f;
		Death();
		return true;
	}
	else if (*currentHealth > maxHealth)
	{
		*currentHealth = maxHealth;
	}
	
	if (Player* _player = dynamic_cast<Player*>(owner))
	{
		_player->UpdateLife();
	}

	return false;
}
