#pragma once
#include "Inventory.h"
#include "PlayerRessource.h"
#include "Living.h"
#include <SFML/Audio.hpp>

using namespace sf;

class Player : public Living
{
	Inventory* inventory;
	PlayerRessource* ressources;
	int* chainMultiplier;
	bool alreadyMoved;
	Menu* life;
	bool isConfuse;


public:
	bool GetIsConfuse() const
	{
		return isConfuse;
	}
	void SetIsConfuse(const bool _isConfuse)
	{
		isConfuse = _isConfuse;
	}
	int GetConfusionEffect()
	{
		return isConfuse ? -1 : 1;
	}
	int* GetChainMultiplier() const
	{
		return chainMultiplier;
	}
	
	PlayerRessource* GetRessources() const
	{
		return ressources;
	}

public:
	//const float _maxHp, const float _maxDammage, const string& _path, const string& _id, const Vector2f& _pos
	Player(const float _maxHp,const float _maxDamages,const string _id, const Vector2f& _position);
	~Player();

public :
	void InitInput();
	void InitLife();

	void Heal();
	void TakeDamage();

	void Update() override;
};

