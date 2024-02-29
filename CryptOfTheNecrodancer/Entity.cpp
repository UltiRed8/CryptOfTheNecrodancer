#include "Entity.h"
#include "Macro.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "AnimationComponent.h"

Entity::Entity(const string& _id, const string& _path, const Vector2f& _position) : IManagable(_id)
{
	Register();
	shape = new RectangleShape(TILE_SIZE);
	shape->setPosition(_position);
	drawables.push_back(shape);
	TextureManager::GetInstance().Load(shape, _path);
	type = ET_NONE;
}

Entity::~Entity()
{
	for (Component* _component : components)
	{
		delete _component;
	}
	components.clear();	
	delete shape;	
}

void Entity::Register()
{
	EntityManager::GetInstance().Add(id, this);
}

void Entity::Update()
{
	if (IsToRemove())
	{
		if (AnimationComponent* _comp = GetComponent<AnimationComponent>())
		{
			for (Animation* _animation : _comp->GetAllValues())
			{
				_animation->Stop();
			}
		}
		return;
	}
	for (Component* _component : components)
	{
		_component->Update();
	}
}
