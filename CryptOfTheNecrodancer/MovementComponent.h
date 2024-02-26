#pragma once
#include<SFML/Graphics.hpp>
#include"Component.h"
#include"EntityManager.h"
#include"Macro.h"
#include"CollisionComponent.h"
#include"Tile.h"
using namespace sf;
class MovementComponent:public Component
{
	int speed;
	bool canMove;
	Vector2f direction;
	Tile* destination;
	CollisionComponent* collision;
	vector<CollisionReaction> reactions;

public:
	bool GetCanMove() const
	{
		return canMove;
	}
	Vector2f GetDirection()const
	{
		return direction;
	}
	void SetDestination( Tile* _destination)
	{
		destination = _destination;
	}

	float GetSpeed() const
	{
		return speed;
	}
	/*Vector2f* GetDestination() const
	{
		return destination;
	};*/
	void SetCanMove(const bool _status)
	{
		canMove = _status;
	}
	void SetDirection(const Vector2f& _direction)
	{
		direction = _direction;
	}
	/*bool IsAtLocation() const
	{
		return Distance(owner->GetPosition(), *destination) <= 10.0f;
	}*/
public:
	MovementComponent(Entity* _owner, const int _speed = 1);


public:
	void InitCollisions(CollisionComponent* _collision, const vector<CollisionReaction>& _reactions);
	void Move();
	void TryToMove();
	void Update();
};
	


