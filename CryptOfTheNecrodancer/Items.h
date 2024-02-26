#pragma once
#include "Entity.h"

class Items : public Entity
{
public:
	Items(const string& _id, const string& _path, const Vector2f& _position);
	Items(const string& _id, const Vector2f& _position);

protected :
	void SetPath(Shape* _shape, const string& _path);

public :
	virtual string GetTexturePath() = 0;
};

