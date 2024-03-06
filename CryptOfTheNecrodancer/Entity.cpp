#include "Entity.h"
#include "Macro.h"
#include "EntityManager.h"
#include "Player.h"

#include "AnimationComponent.h"

Entity::Entity(const string& _id, const string& _path, const Vector2f& _position) : IManagable(_id)
{
	Register();
	shape = new RectangleShape(TILE_SIZE);
	shape->setPosition(_position);

	SetTexture(_path);
	type = ET_NONE;
}

Entity::~Entity()
{
	for (Component* _component : components)
	{
		if (AnimationComponent* _animationComponent = dynamic_cast<AnimationComponent*>(_component))
		{
			for (Animation* _animation : _animationComponent->GetAllValues())
			{
				_animation->Stop();
				_animation->Destroy();
			}
		}
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
	if (AnimationComponent* _animationComponent = GetComponent<AnimationComponent>())
	{
		_animationComponent->GetCurrent()->Replay();
	}
	for (Component* _component : components)
	{
		Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
		if (_player->GetIsStun() && _component == _player->GetComponent<MovementComponent>())
		{
			return;
		}
		_component->Update();
	}
}