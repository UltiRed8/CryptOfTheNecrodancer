#pragma once
#include "Entity.h"


#define PATH_STAIR "stairs.png"

class Stair : public Entity
{
	
public:
	Stair(const string& _path, const Vector2f& _position);
};

