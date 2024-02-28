#pragma once
#include "Entity.h"


#define PATH_TRAPDOOR "stairs.png"


class Trap : public Entity
{
public:
	Trap(const string& _path, const Vector2f& _position);
};

