#include "Player.h"
#include "Macro.h"
#include "InputManager.h"

#define PLAYER_PATH "Assets/Textures/Player.png"

Player::Player(const string _id, const Vector2f& _position, PlayerRessource _ressources) : Entity(_id, PLAYER_PATH, _position)
{
	inventory = new Inventory();
	ressources = _ressources;
}

Player::~Player()
{
	delete inventory;
}

void Player::InitInput()
{
	
}
