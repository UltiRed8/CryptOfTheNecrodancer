#include "InputManager.h"
#include "EntityManager.h"
#include "InputData.h"
#include "ActionMap.h"
#include "Clickable.h"

bool InputManager::Update(RenderWindow* _window)
{
	Event _event;
	while (_window->pollEvent(_event))
	{
		if (_event.type == Event::Closed)
		{
			return false;
		}
		UpdateInputs(_event);
	}
	mousePosition = Vector2f(Mouse::getPosition(*_window));
	GarbageCollector();
	return true;
}

void InputManager::UpdateInputs(const Event& _event)
{
	for (ActionMap* _map : GetAllValues())
	{
		_map->Update(_event);
	}
}

void InputManager::ClickOnEntities()
{
	for (Entity* _entity : EntityManager::GetInstance()->GetAllValues())
	{
		if (Clickable* _clickable = dynamic_cast<Clickable*>(_entity))
		{
			if (_clickable->GetGlobalBounds().contains(mousePosition))
			{
				_clickable->ClickAction();
			}
		}
	}
}
