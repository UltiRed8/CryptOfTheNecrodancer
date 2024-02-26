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

void ActionMap::Register()
{
	InputManager::GetInstance()->Add(id, this);
}

void ActionMap::Update(const Event& _event)
{
	for (Action* _action : GetAllValues())
	{
		for (const InputTypeData& _inputData : _action->GetData().keys)
		{
			if (_event.type == _inputData.type && _event.key.code == _inputData.key)
			{
				_action->Execute();
			}
		}
	}
	GarbageCollector();
}