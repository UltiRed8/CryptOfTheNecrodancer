#pragma once
#include "Entity.h"


class Tile : public Entity
{
	vector<Entity*> entities;
	Color originColor;

public:
	Tile(const string& _tilePath,const Vector2f& _position);

	void SetOriginColor(const Color& _color)
	{
		originColor = _color;
		shape->setFillColor(originColor);
	}

	// Hérité via Entity
	void Update() override;

	void ResetColor();
};

