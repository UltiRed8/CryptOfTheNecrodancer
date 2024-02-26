#include "Player.h"
#include "Macro.h"
#include "InputManager.h"
#define PATH_PLAYER ""
Player::Player(const string _id, const Vector2f& _position, PlayerRessource _ressources) : Entity(_id, PATH_PLAYER,_position)
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
