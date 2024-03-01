#pragma once
#include "IManagable.h"
#include "Component.h"
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "AnimationComponent.h"

using namespace std;
using namespace sf;

enum EntityType
{
	ET_NONE, ET_FLOOR, ET_WALL, ET_ENEMY, ET_PLAYER, ET_ITEM, ET_STAIR, ET_TRAP,ET_SHADOW, ET_DOOR,ET_PICKABLE,ET_EPHAESTUS
};


class Entity : public IManagable<string>
{
protected:
	vector<Component*> components;
	Shape* shape;
	EntityType type;
	int zIndex;

public:
	EntityType GetType() const
	{
		return type;
	}
	Shape* GetShape() const 
	{ 
		return shape;
	}

	int GetZIndex() const
	{
		return zIndex;
	}
	template <typename Type>
	Type* GetComponent()
	{
		for (Component* _component : components)
		{
			if (Type* _type = dynamic_cast<Type*>(_component))
			{
				return _type;
			}
		}
		return nullptr;
	}
	vector<Drawable*> GetDrawables()
	{
		if (AnimationComponent* _anim = GetComponent<AnimationComponent>())
		{
			return _anim->GetDrawables();
		}
		vector<Drawable*> _drawables;
		_drawables.push_back(shape);
		return _drawables;
	}

	Vector2f GetPosition()const 
	{
		return shape->getPosition();
	}

	void SetTexture(const string& _path)
	{
		TextureManager::GetInstance().Load(shape, _path);
	}

public:
	Entity(const string& _id,const string& _path, const Vector2f& _position);
	~Entity();

	void Register() override;
	virtual void Update();
};

