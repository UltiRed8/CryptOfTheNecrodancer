#include "Enemy.h"

Enemy::Enemy(const string& _id,const string& _path,const Vector2f& _position):Entity(_id,_path,_position)
{
	indexPatern = 0;
	zIndex = 2;

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
		});
	components.push_back(_collision);
	components.push_back(_movement);

	type = ET_ENEMY;
}
