#include "Tile.h"
#include "Macro.h"
Tile::Tile(const string& _tilePath, const Vector2f& _position) : Entity(STRING_ID("Tile"), _tilePath, _position)
{
	entities = vector<Entity*>();
}

void Tile::InvertColors()
{
	Color _currentColor = shape->getFillColor();
	if (firstColor == _currentColor)
	{
		shape->setFillColor(secondColor);
	}
	else
	{
		shape->setFillColor(firstColor);
	}
}

void Tile::Update()
{
}

void Tile::ResetColor()
{
	shape->setFillColor(firstColor);
	shape->setOutlineThickness(0.f);
}
