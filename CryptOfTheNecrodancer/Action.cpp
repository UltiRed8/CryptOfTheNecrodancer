#include "Action.h"
#include "ActionMap.h"
#include "InputManager.h"

ActionData::ActionData()
{
	name = "Unknown";
	keys = vector<InputTypeData>();
	callback = function<void()>();
}

Action::Action(const ActionData& _data, const string& _actionMap) : IManagable(_data.name)
{
	if (!_data)
	{
		SetToRemove();
	}
	else
	{
		data = _data;
		actionMap = _actionMap;
	}
	Register();
}

void Action::Register()
{
	if (ActionMap* _actionMap = InputManager::GetInstance()->Get(actionMap))
	{
		_actionMap->Add(id, this);
		return;
	}

	ActionMap* _newMap = new ActionMap(actionMap);
	_newMap->Add(id, this);
}

bool Action::ContainsKey(const int _keyToFind) const
{
	for (const InputTypeData& _key : data.keys)
	{
		if (_key.key == _keyToFind)
		{
			return true;
		}
	}
	return false;
}

bool Action::IsEmpty() const
{
	return data.keys.empty();
}

void Action::Execute()
{
	if (data.callback)
	{
		data.callback();
	}
}