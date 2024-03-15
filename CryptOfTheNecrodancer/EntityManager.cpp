#include "EntityManager.h"
#include "AnimationComponent.h"
#include "Item.h"

void EntityManager::Update()
{
	for (Entity* _entity : GetAllValues())
	{
		_entity->Update();
	}
	GarbageCollector();
}

void EntityManager::ItemUpdate()
{
	float _delta = TimerManager::GetInstance().GetDeltaTime();
	for (Entity* _entity : GetAllValues())
	{
		if (_entity->GetType() == ET_ITEM)
		{
			if (Item* _item = (Item*)_entity)
			{
				_item->GetAnimation()->Update(_delta);
			}
		}
	}
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
	int _currentZIndex = 0;
	bool _shouldContinue;
	do
	{
		_shouldContinue = false;
		for (Entity* _entity : GetAllValues())
		{
			if (_entity->IsToRemove()) continue;
			int _zIndex = _entity->GetZIndex();
			if (_zIndex == _currentZIndex)
			{
				 vector<Drawable*> _tempDrawables = _entity->GetDrawables();
				_drawables.insert(_drawables.end(), _tempDrawables.begin(), _tempDrawables.end());
			}
			else if (_zIndex == _currentZIndex + 1)
			{
				_shouldContinue = true;
			}
		}
		_currentZIndex++;
	} while (_shouldContinue);
	
	return _drawables;
}