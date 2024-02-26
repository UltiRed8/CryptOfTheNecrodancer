#include "Player.h"
#include "Macro.h"
#include "InputManager.h"
#include "MovementComponent.h"

#define PATH_PLAYER ""

Player::Player(const string _id, const Vector2f& _position, PlayerRessource _ressources) : Entity(_id, PATH_PLAYER,_position)
{
	inventory = new Inventory();
	ressources = _ressources;
	components.push_back(new MovementComponent(this));
	InitInput();
	zIndex = 1;
}

Player::~Player()
{
	delete inventory;
}

void Player::InitInput()
{
	new ActionMap("Mouvements",
		{ ActionData("Haut", [this]() { GetComponent<MovementComponent>()->SetDirection(new Vector2i(0.f,-1.f)); }, {Event::KeyPressed, Keyboard::Up}),
		  ActionData("Bas", [this]() { GetComponent<MovementComponent>()->SetDirection(new Vector2i(0, 1)); }, {Event::KeyPressed, Keyboard::Down}),
		  ActionData("Droite", [this]() { GetComponent<MovementComponent>()->SetDirection(new Vector2i(1, 0)); }, {Event::KeyPressed, Keyboard::Right}),
		  ActionData("Gauche", [this]() { GetComponent<MovementComponent>()->SetDirection(new Vector2i(-1, 0)); }, {Event::KeyPressed, Keyboard::Left})
		});
}

void Player::Update()
{
	Entity::Update();
}
