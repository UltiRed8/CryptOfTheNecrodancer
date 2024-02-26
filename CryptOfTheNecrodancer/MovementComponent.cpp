#include "MovementComponent.h"
#include"TimerManager.h"
MovementComponent::MovementComponent(Entity* _owner, const int _speed ):Component(_owner)
{
	canMove = true;
	direction = Vector2f(0.f, 0.f);
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
	if (!canMove/* || !destination*/) return;
	/*if (IsAtLocation())
	{
		delete destination;
		destination = nullptr;
		direction = Vector2f(0.0f, 0.0f);
		return;
	}*/
	const float _deltaTime = TimerManager::GetInstance()->GetDeltaTime();
	Shape* _shape = owner->GetShape();
	//const Vector2f& _tileSize = Map::GetInstance().GetTileSize();
	/*const float _directionX = (direction.x * speed * _tileSize.x ) ;
	const float _directionY = (direction.y * speed * _tileSize.y) ;
	const Vector2f& _position = owner->GetPosition() + Vector2f(_directionX, _directionY);
	_shape->setPosition(_position);*/
}



void MovementComponent::TryToMove()
{
	Move();
	if (!collision)return;
	//CheckCollision
	

}

void MovementComponent::Update()
{
	TryToMove();
}
