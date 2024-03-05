#include "ActionMap.h"
#include "InputManager.h"

ActionMap::ActionMap(const string& _name, const vector<ActionData>& _actions) : IManagable(_name)
{
	Register();

	for (const ActionData& _actionData : _actions)
	{
		actions.push_back(new Action(_actionData, id));
	}
}

ActionMap::~ActionMap()
{
	for (Action* _action : GetAllValues())
	{
		_action->Destroy();
	}
}

void ActionMap::Register()
{
	InputManager::GetInstance().Add(id, this);
}

void ActionMap::Update(const Event& _event)
{
	for (Action* _action : GetAllValues())
	{
		for (const InputTypeData& _inputData : _action->GetData().keys)
		{
			if (_event.type == _inputData.type && _event.text.unicode == _inputData.keyChar)
			{
				_action->Execute();
			}
		}
	}
	GarbageCollector();
}