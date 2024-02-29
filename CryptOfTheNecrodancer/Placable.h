#pragma once
#include "Entity.h"

class Placable : public Entity
{
public:
	Placable(const string& _id, const string& _path,const Vector2f& pos);
};

