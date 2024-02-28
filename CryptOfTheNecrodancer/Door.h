#pragma once

#include "Entity.h"

class Door : public Entity
{
	int direction;

public:
	Door(const Vector2f& _position, const int _direction = -1);

private:
	void UpdateTexture();
	
public:
	void OpenDoor();
	void ComputeDirection();
};