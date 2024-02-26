#pragma once

#include "Singleton.h"
#include "IManager.h"
#include "Entity.h"

using namespace std;

class EntityManager : public Singleton<EntityManager>, public IManager<string, Entity>
{
public:
	void Update();
	bool IsAvailable(const Vector2f& _position, const Vector2f& _size);
	vector<Drawable*> GetDrawables() const;
	template <typename Type>
	vector<Type*> GetAllOfType()
	{
		vector<Type*> _entities;
		for (Entity* _entity : GetAllValues())
		{
			if (Type* _type = dynamic_cast<Type*>(_entity))
			{
				_entities.push_back(_type);
			}
		}
		return _entities;
	}
};