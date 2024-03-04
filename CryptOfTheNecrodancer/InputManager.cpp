#include "InputManager.h"
#include "EntityManager.h"
#include "InputData.h"
#include "ActionMap.h"
#include "WindowManager.h"

bool InputManager::Update()
{
	RenderWindow* _window = WindowManager::GetInstance().GetWindow();
	Event _event;
	while (_window->pollEvent(_event))
	{
		if (_event.type == Event::Closed)
		{
			_window->close();
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