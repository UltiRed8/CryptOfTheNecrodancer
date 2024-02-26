#pragma once
#include "Inventory.h"
#include "PlayerRessource.h"
#include "Entity.h"

class Player : public Entity
{
	Inventory* inventory;
	PlayerRessource ressources;

public :
	Player(const string _id, const Vector2f& _position, PlayerRessource _ressources);
	~Player();

public :
	void InitInput();

	void Move();

	// Hérité via Entity
	void Update() override;
};

