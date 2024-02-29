#include "Enemy.h"

Enemy::Enemy(const float _maxHp, const float _maxDammage, const string& _id, const string& _path, const Vector2f& _position, const int _droppedCoins)
	: Living(_maxHp,_maxDammage, _path, _id,_position)
{
	indexPatern = 0;
	zIndex = 1;
	droppedCoins = _droppedCoins;
	currentCooldown = 0;
	cooldown = 0;

	MovementComponent* _movement =	GetComponent<MovementComponent>();
	CollisionComponent* _collision = new CollisionComponent(this);
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
	components.push_back(_collision);
	components.push_back(_movement);

	type = ET_ENEMY;
}

void Enemy::DieEvent()
{
	new Pickable(droppedCoins, PT_COIN,STRING_ID("coin"), GetPosition());
	this->Destroy();
}