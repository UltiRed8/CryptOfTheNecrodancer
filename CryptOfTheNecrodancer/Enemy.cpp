#include "Enemy.h"
#include "Coin.h"

Enemy::Enemy(const string& _id,const string& _path,const Vector2f& _position, const int _droppedCoins) : Entity(_id,_path,_position)
{
	indexPatern = 0;
	zIndex = 1;
	droppedCoins = _droppedCoins;
	currentCooldown = 0;
	cooldown = 0;

	MovementComponent* _movement = new MovementComponent(this);
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

void Enemy::InitAllBattlesComponents(const float _damagesAmounts, const float _maxHealth)
{
	components.push_back(new DamageComponent(this, _damagesAmounts));
	components.push_back(new LifeComponent(this, [this]() {DieEvent(); }, false, _maxHealth));
}

void Enemy::DieEvent()
{
	new Coin(droppedCoins, "coin", GetPosition());
	this->Destroy();
}