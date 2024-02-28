#pragma once
#include "Entity.h"


class Tile : public Entity
{
	vector<Entity*> entities;
	Color firstColor;
	Color secondColor;
	Color lastColor;

public:
	void SetColors(const Color& _firstColor, const Color& _secondColor)
	{
		firstColor = _firstColor;
		secondColor = _secondColor;
		shape->setFillColor(_firstColor);
	}

public:
	Tile(const string& _tilePath,const Vector2f& _position,const EntityType& _type = ET_FLOOR);

public:
	void InvertColors();

	// Hérité via Entity
	void Update() override;

	void ResetColor();
};

