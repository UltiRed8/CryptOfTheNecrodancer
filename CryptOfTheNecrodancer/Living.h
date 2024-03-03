#pragma once
#include "Entity.h"

class Living : public Entity
{

public:
	Living(const float _maxHp,const float _maxDammage,const string& _path,const string& _id,const Vector2f& _pos);
	Living(const string& _path,const string& _id,const Vector2f& _pos);

public:
	virtual void DieEvent() = 0;
};

