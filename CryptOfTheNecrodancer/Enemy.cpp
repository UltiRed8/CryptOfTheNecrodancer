#include "Enemy.h"
#include "LightSource.h"
Enemy::Enemy(const float _maxHp, const float _maxDammage, const string& _id, const string& _path, const Vector2f& _position, const int _droppedCoins)
	: Living(_maxHp,_maxDammage, _path, _id,_position)
{
	indexPatern = 0;
	zIndex = 2;
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
	rewardAmount = _droppedCoins;
	new LightSource(STRING_ID("lightsource"), this, 100);
	type = ET_ENEMY;
}
//Tu part de la fin des coeur 

void Enemy::DieEvent()
{
	new Pickable(rewardAmount, PT_COIN,STRING_ID("coin"), GetPosition());
	this->Destroy();
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