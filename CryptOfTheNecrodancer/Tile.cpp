#include "Tile.h"
#include "Macro.h"
Tile::Tile(const string& _tilePath, const Vector2f& _position) : Entity(STRING_ID("Tile"), _tilePath, _position)
{
	entities = vector<Entity*>();
	type = ET_FLOOR;
}

void Tile::InvertColors()
{
	Color _currentColor = shape->getFillColor();
	if (firstColor == _currentColor)
	{
		shape->setFillColor(secondColor);
		lastColor = secondColor;
	}
	else
	{
		shape->setFillColor(firstColor);
		lastColor = firstColor;
	}
}

void Tile::Update()
{
}

void Tile::ResetColor()
{
	shape->setFillColor(lastColor);
	shape->setOutlineThickness(0.f);
}
