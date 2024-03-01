#include "Enemy.h"
#include "Heart.h"

Enemy::Enemy(const float _maxHp, const float _maxDammage, const string& _id, const string& _path, const Vector2f& _position, const int _droppedCoins)
	: Living(_maxHp,_maxDammage, _path, _id,_position)
{
	indexPatern = 0;
	zIndex = 2;
	droppedCoins = _droppedCoins;
	currentCooldown = 0;
	cooldown = 0;

	MovementComponent* _movement = GetComponent<MovementComponent>();
	CollisionComponent* _collision = new CollisionComponent(this);
	components.push_back(_collision);
	_movement->InitCollisions(_collision, {
		CollisionReaction(ET_WALL, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
		}),
		CollisionReaction(ET_ENEMY, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
		}),
		CollisionReaction(ET_PLAYER, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			GetComponent<DamageComponent>()->Attack(_entity);
		}),
	});

	type = ET_ENEMY;
}

void Enemy::DieEvent()
{
	new Pickable(droppedCoins, PT_COIN,STRING_ID("coin"), GetPosition());
	this->Destroy();
}

void Enemy::InitLifeUI()
{
	//TODO Changer position
	Menu* _ennmyLife = new Menu("PlayerLife", {
		/*new Heart(STRING_ID("Hearts"), Vector2f(25.0f, 25.0f) * 2.0f, Vector2f(SCREEN_WIDTH - 55 * 4.2f, SCREEN_HEIGHT - 55 * 12.8)),
		new Heart(STRING_ID("Hearts"), Vector2f(25.0f, 25.0f) * 2.0f, Vector2f(SCREEN_WIDTH - 55 * (4.2f + 1.0f), SCREEN_HEIGHT - 55 * 12.8)),
		new Heart(STRING_ID("Hearts"), Vector2f(25.0f, 25.0f) * 2.0f, Vector2f(SCREEN_WIDTH - 55 * (4.2f + 2.0f), SCREEN_HEIGHT - 55 * 12.8)),*/
		}, 7, false);

	_ennmyLife->Open();
}

void Enemy::Update()
{
	currentCooldown--;
	if (currentCooldown <= 0)
	{
		currentCooldown = cooldown;
		SelectDirection();
	}
	Entity::Update();

}