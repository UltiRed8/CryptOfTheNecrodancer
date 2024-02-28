#pragma once
#include "Inventory.h"
#include "PlayerRessource.h"
#include "Entity.h"

#include <SFML/Audio.hpp>

using namespace sf;

class Player : public Entity
{
	Inventory* inventory;
	PlayerRessource ressources;
	// Listener* listener;
	float chainMultiplier;

public:
	float GetChainMultiplier() const
	{
		return chainMultiplier;
	}

public:
	Player(const string _id, const Vector2f& _position, PlayerRessource _ressources);
	~Player();

public :
	void InitInput();

	// Hérité via Entity
	void Update() override;
};

