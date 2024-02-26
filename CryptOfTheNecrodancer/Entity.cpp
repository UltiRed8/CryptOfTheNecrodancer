#include "Entity.h"
#include "Macro.h"
#include "EntityManager.h"
#include "TextureManager.h"

Entity::Entity(const string& _id, const string& _path, const Vector2f& _position) : IManagable(STRING_ID(_id))
{
	Register();
	shape = new RectangleShape(TILE_SIZE);
	shape->setPosition(_position);
	drawables.push_back(shape);
	TextureManager::GetInstance()->Load(shape, _path);
}

Entity::~Entity()
{
	delete shape;
}

void Entity::Register()
{
	EntityManager::GetInstance()->Add(id, this);
}
