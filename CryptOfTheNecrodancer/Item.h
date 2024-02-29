#pragma once
#include "Entity.h"


class Item : public Entity
{
public:
	Item(const string& _id, const string& _path, const Vector2f& _position);
	Item(const string& _id, const Vector2f& _position);

protected :
	void SetPath(Shape* _shape, const string& _path);

public :
	virtual string GetTexturePath() = 0;
};

