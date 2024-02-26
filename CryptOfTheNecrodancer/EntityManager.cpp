#include "EntityManager.h"

void EntityManager::Update()
{
	vector<Entity*> _entities = GetAllValues();
	for (Entity* _entity : GetAllValues())
	{
		_entity->Update();
	}
	GarbageCollector();
}

bool EntityManager::IsAvailable(const Vector2f& _position, const Vector2f& _size)
{
	const FloatRect& _rect = FloatRect(_position - _size/2.0f, _size);
	for (auto _pair : allValues)
	{
		if (_pair.second->GetShape()->getLocalBounds().intersects(_rect)) return true;
	}
	return false;
}

vector<Drawable*> EntityManager::GetDrawables() const
{
	vector<Drawable*> _drawables;
	for (auto _pair : allValues)
	{
		/*vector<Drawable*> _drawablesTemp = _pair.second->GetDrawables();
		_drawables.insert(_drawables.end(), _drawablesTemp.begin(), _drawablesTemp.end());
		if (Human* _human = dynamic_cast<Human*>(_pair.second))
		{
			if (_human->GetJob() && !_human->IsInfected())
			{
				_drawables.push_back(_human->GetJob()->GetShape());
			}
		}*/
	}
	return _drawables;
}