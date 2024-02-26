#include "Entity.h"
#include "Macro.h"

Entity::Entity(const string& _id, const Vector2f& _position) : IManagable(STRING_ID(_id))
{
	shape = new RectangleShape(TILE_SIZE);
	shape->setPosition(_position);
	drawables.push_back(shape);
}

Entity::~Entity()
{
	delete shape;
}
