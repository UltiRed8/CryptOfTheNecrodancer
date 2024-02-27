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
	Vector2i* direction;
	CollisionComponent* collision;
	vector<CollisionReaction> reactions;

public:
	bool GetCanMove() 
	{
		canMove = true;
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
	void SetDirection(const Vector2i& _direction, const bool _isPlayer = true)
	{
		if (!canMove) return;
		if (direction)
		{
			delete direction;
		}
		direction = new Vector2i(_direction);
		if (_isPlayer)
		{
			canMove = false;
		}
	}

	
public:
	MovementComponent(Entity* _owner, const int _speed = 1);


public:
	void InitCollisions(CollisionComponent* _collision, const vector<CollisionReaction>& _reactions);
	void Move();
	void TryToMove();
	void Update();
};
	


