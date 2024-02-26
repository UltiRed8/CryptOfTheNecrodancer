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
	bool GetCanMove() const
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
	void SetDirection(Vector2i* _direction,bool _canMove = true)
	{
		direction = _direction;
		canMove = _canMove;
	}
	
public:
	MovementComponent(Entity* _owner, const int _speed = 1);


public:
	void InitCollisions(CollisionComponent* _collision, const vector<CollisionReaction>& _reactions);
	void Move();
	void TryToMove();
	void Update();
};
	


