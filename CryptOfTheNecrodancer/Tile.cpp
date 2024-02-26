#include "Tile.h"
#include "Macro.h"
Tile::Tile(const string& _tilePath, const Vector2f& _position) : Entity(STRING_ID("Tile"), _tilePath, _position)
{
	entities = vector<Entity*>();
}

void Tile::Update()
{
}

void Tile::ResetColor()
{
	shape->setFillColor(originColor);
	shape->setOutlineThickness(0.f);
}
