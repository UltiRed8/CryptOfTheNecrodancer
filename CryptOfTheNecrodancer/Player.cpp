#include "Player.h"
#include "Macro.h"
#include "InputManager.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"

#define PATH_PLAYER "PlayerSprite.png"

Player::Player(const string _id, const Vector2f& _position, PlayerRessource _ressources) : Entity(_id, "", _position)
{
	inventory = new Inventory();
	ressources = _ressources;
	components.push_back(new MovementComponent(this));
	AnimationData _animation = AnimationData("Idle", Vector2f(0, 0),TILE_SIZE, READ_RIGHT,ANIM_DIR_NONE,true,4,0.1f);
	//components.push_back(new AnimationComponent(this, PATH_PLAYER, { _animation }, ANIM_DIR_NONE));
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
		{ ActionData("Haut", [this]() { GetComponent<MovementComponent>()->SetDirection(new Vector2i(0,-1)); }, {Event::KeyPressed, Keyboard::Up}),
		  ActionData("Bas", [this]() { GetComponent<MovementComponent>()->SetDirection(new Vector2i(0, 1)); }, {Event::KeyPressed, Keyboard::Down}),
		  ActionData("Droite", [this]() { GetComponent<MovementComponent>()->SetDirection(new Vector2i(1, 0)); }, {Event::KeyPressed, Keyboard::Right}),
		  ActionData("Gauche", [this]() { GetComponent<MovementComponent>()->SetDirection(new Vector2i(-1, 0)); }, {Event::KeyPressed, Keyboard::Left})
		});
}

void Player::Update()
{
	Entity::Update();
}
