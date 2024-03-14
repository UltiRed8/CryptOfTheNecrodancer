#include "LifeComponent.h"
#include "Entity.h"
#include "Player.h"
#include "Item.h"
#include <fstream>
#define EMPTY "UI/HealthBarEmpty.png"
#define FULL "UI/HealthBarFull.png"

LifeComponent::LifeComponent(Entity* _owner, const function<void()> _deathCallback, const bool _invulnerable, const float _maxHealth, bool _isPlayer) : Component(_owner)
{
	invulnerable = _invulnerable;
	maxHealth = _maxHealth;
	if (owner->GetType() == ET_PLAYER)
	{
		ifstream _stream = ifstream("Assets/Saved/PlayerStats.txt");
		if (_stream)
		{
			string _line;
			getline(_stream, _line);
			getline(_stream, _line);
			maxHealth = stof(_line);
		}
	}
	currentHealth = new float(maxHealth);
	deathcallback = _deathCallback;
	isAlive = true;
	if (_isPlayer) return;
	pBPos = Vector2f(_owner->GetPosition().x, _owner->GetPosition().y - TILE_SIZE.y / 2.f);
	healthBar = new ProgressBar(STRING_ID("LifeEnemy"), PT_LEFT, pBPos, Vector2f(50.0f, 10.0f), EMPTY, FULL, currentHealth, maxHealth);
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

void LifeComponent::SetMaxHealth(const float _newHealth)
{
	maxHealth = _newHealth;
}



void LifeComponent::SetChangeCurrentHealth(float _byAmount)
{
	*currentHealth = _byAmount;
}

bool LifeComponent::ChangeHealth(const float _byAmount)
{
	if (invulnerable) return false;

	float _defense = 1.0f;
	if (owner->GetType() == ET_PLAYER)
	{
		if (Player* _player = (Player*)owner)
		{
			for (Slot* _slot : _player->GetInventory()->GetSlots())
			{
				if (Item* _item = _slot->GetItem())
				{
					_defense += _item->GetStats().defense;
				}
			}
		}
	}

	float _value = abs(_byAmount / _defense);
	_value = _value < 50.0f ? 50.0f : _value;

	_value = round(_value / 50.0f) * 50.0f;

	_value *= _byAmount < 0.0f ? -1.0f : 1.0f;


	*currentHealth += _value;
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
	//if (Player* _player = dynamic_cast<Player*>(owner))
	//{
	//	_player->UpdateLife();
	//}
	return false;
}