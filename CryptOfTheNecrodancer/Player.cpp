#include "Player.h"
#include "Macro.h"
#include "InputManager.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"
#include "RythmComponent.h"

#define PATH_PLAYER "PlayerSprite.png"

Player::Player(const string _id, const Vector2f& _position, PlayerRessource _ressources) : Entity(_id, "", _position)
{
	inventory = new Inventory();
	ressources = _ressources;
	components.push_back(new MovementComponent(this));
	AnimationData _animation = AnimationData("Idle", Vector2f(0, 0),Vector2f(26,26), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
	components.push_back(new AnimationComponent(this, PATH_PLAYER, { _animation }, ANIM_DIR_NONE));
	InitInput();
	zIndex = 1;
	chainMultiplier = 1.0f;
}

Player::~Player()
{
	delete inventory;
}

void Player::InitInput()
{
	new ActionMap("Mouvements",
		{ ActionData("Haut", [this]() { GetComponent<MovementComponent>()->SetDirection(Vector2i(0,-1)); cout << "t" << endl; }, {Event::KeyPressed, Keyboard::Up}),
		  ActionData("Bas", [this]() { GetComponent<MovementComponent>()->SetDirection(Vector2i(0, 1)); cout << "t" << endl; }, {Event::KeyPressed, Keyboard::Down}),
		  ActionData("Droite", [this]() { GetComponent<MovementComponent>()->SetDirection(Vector2i(1, 0)); cout << "t" << endl; }, {Event::KeyPressed, Keyboard::Right}),
		  ActionData("Gauche", [this]() { GetComponent<MovementComponent>()->SetDirection(Vector2i(-1, 0)); cout << "t" << endl; }, {Event::KeyPressed, Keyboard::Left})
		});

	// TODO remove
	new ActionMap("TempDebug",
		{ ActionData("Decrease", [this]() { chainMultiplier = 1.0f; cout << "Set chain multiplier to: 1.0f!" << endl; }, {Event::KeyPressed, Keyboard::Num1}),
		  ActionData("Increase", [this]() { chainMultiplier = 2.0f; cout << "Set chain multiplier to: 2.0f!" << endl; }, {Event::KeyPressed, Keyboard::Num2}),
		});
}

void Player::Update()
{
	Entity::Update();
	GetComponent<AnimationComponent>()->Update();
}
