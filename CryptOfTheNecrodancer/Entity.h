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
	ET_NONE, ET_FLOOR, ET_WALL, ET_ENEMY, ET_PLAYER, ET_ITEM, 
	ET_STAIR, ET_TRAP, ET_SHADOW, ET_DOOR, ET_PICKABLE, ET_NPC, 
	ET_WATER, ET_ICE, ET_HOTCOALS
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

	void SetZIndex(int _zIndex)
	{
		zIndex = _zIndex;
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
	virtual vector<Drawable*> GetDrawables()
	{
		if (AnimationComponent* _anim = GetComponent<AnimationComponent>())
		{
			return _anim->GetDrawables();
		}
		vector<Drawable*> _drawables;
		_drawables.push_back(shape);
		return _drawables;
	}

	void AddComponent(Component* _component)
	{
		components.push_back(_component);
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
	virtual ~Entity();

	void Register() override;
	virtual void Update();
};