#include "MovementComponent.h"
#include"TimerManager.h"
MovementComponent::MovementComponent(Entity* _owner, const int _speed ):Component(_owner)
{
	canMove = true;
	direction = Vector2i(0, 0);
	collision = nullptr;
	reactions = vector<CollisionReaction>();
	speed = _speed;
}


void MovementComponent::InitCollisions(CollisionComponent* _collision, const vector<CollisionReaction>& _reactions)
{
	collision = _collision;
	reactions = _reactions;
}

void MovementComponent::Move()
{
	if (!canMove || !destination) return;
	if (IsAtLocation())
	{
		delete destination;
		destination = nullptr;
		direction = Vector2i(0, 0);
		return;
	}
	const float _deltaTime = TimerManager::GetInstance().GetDeltaTime();
	Shape* _shape = owner->GetShape();
	const Vector2f& _tileSize = TILE_SIZE;
	const float _directionX = (direction.x * speed * _tileSize.x ) ;
	const float _directionY = (direction.y * speed * _tileSize.y) ;
	const Vector2f& _position = owner->GetPosition() + Vector2f(_directionX, _directionY);
	_shape->setPosition(_position);
}



void MovementComponent::TryToMove()
{
	Move();
	if (!collision)return;
	collision->CheckCollision(reactions);
	

}

void MovementComponent::Update()
{
	TryToMove();
}
