#pragma once
#include "Entity.h"

class Placeable : public Entity
{
public:
	Placeable(const string& _id, const string& _path,const Vector2f& pos);
};

