#pragma once
#include "Entity.h"


class Tile : public Entity
{
	vector<Entity*> entities;
	Color defaultColor;
	Color baseColor;

public:
	void SetColors(const Color& _baseColor)
	{
		defaultColor = _baseColor;
		baseColor = _baseColor;
		shape->setFillColor(defaultColor);
	}

public:
	Tile(const string& _tilePath,const Vector2f& _position);

public:
	void InvertAlpha(const bool _reset);
	void ToggleHighlight(const int _alphaValueToHighlight);
	void Update() override;
};

