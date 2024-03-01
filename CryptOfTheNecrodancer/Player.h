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
	bool isConfuse;

public:
	bool GetIsConfuse() const
	{
		return isConfuse;
	}
	void SetIsConfuse(const bool _isConfuse)
	{
		isConfuse = _isConfuse;
		new Timer("ResetConfusion", [this]() {
			isConfuse = false;
			}, seconds(6.5f), 1, true);
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
	void Update() override;
};

