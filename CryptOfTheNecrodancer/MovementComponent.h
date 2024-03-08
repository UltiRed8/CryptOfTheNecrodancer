#pragma once
#include<SFML/Graphics.hpp>
#include"Component.h"
#include"EntityManager.h"
#include"Macro.h"
#include"CollisionComponent.h"
#include"Tile.h"

using namespace sf;

class MovementComponent :public Component
{
	int speed;
	bool canMove;
	Vector2i* direction;
	CollisionComponent* collision;
	vector<CollisionReaction> reactions;
	Vector2f oldPosition;
	Vector2i oldDirection;
	bool shouldResetDirection;

public:
	bool GetCanMove()
	{
		return canMove;
	}
	int GetSpeed() const
	{
		return speed;
	}
	void SetCanMove(const bool _status)
	{
		canMove = _status;
	}
	void SetDirection(const Vector2i& _direction)
	{
		if (direction)
		{
			delete direction;
		}
		direction = new Vector2i(_direction);
	}
	Vector2i* GetDirection()
	{
		return direction;
	}
	Vector2i GetOldDirection()  
	{
		return oldDirection;
	}

public:
	MovementComponent(Entity* _owner, const int _speed = 1, const bool _shouldResetDirection = true);

public:
	void UndoMove();
	void InitCollisions(CollisionComponent* _collision, const vector<CollisionReaction>& _reactions);
	void Move();
	void TryToMove();
	void Update();
};