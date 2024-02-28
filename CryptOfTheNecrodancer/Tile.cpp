#include "Tile.h"
#include "Macro.h"

#define C_PURPLE Color(255, 62, 216, 255)
#define C_GREEN Color(53, 233, 136, 255)

Tile::Tile(const string& _tilePath, const Vector2f& _position) : Entity(STRING_ID("Tile"), _tilePath, _position)
{
	entities = vector<Entity*>();
	type = ET_FLOOR;
}

void Tile::InvertAlpha(const bool _reset)
{
	if (_reset)
	{
		baseColor = defaultColor;
		shape->setFillColor(baseColor);
		return;
	}
	baseColor.a = baseColor.a == 255 ? 200 : 255;
	shape->setFillColor(baseColor);
}

void Tile::ToggleHighlight(const int _alphaValueToHighlight)
{
	if (defaultColor.a == _alphaValueToHighlight)
	{
		shape->setFillColor(defaultColor.a == 255 ? C_PURPLE : C_GREEN);
	}
	else
	{
		shape->setFillColor(defaultColor);
	}
}

void Tile::Update()
{
}
