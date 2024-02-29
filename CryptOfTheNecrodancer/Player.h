#pragma once
#include "Inventory.h"
#include "PlayerRessource.h"
#include "Entity.h"

#include <SFML/Audio.hpp>

using namespace sf;

class Player : public Entity
{
	Inventory* inventory;
	PlayerRessource* ressources;
	int* chainMultiplier;
	bool alreadyMoved;

public:
	int* GetChainMultiplier() const
	{
		return chainMultiplier;
	}
	
	PlayerRessource* GetRessources() const
	{
		return ressources;
	}

public:
	Player(const string _id, const Vector2f& _position);
	~Player();

public :
	void InitInput();
	void Update() override;
};

