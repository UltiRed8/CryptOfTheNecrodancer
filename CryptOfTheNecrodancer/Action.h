#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "IManagable.h"

using namespace std;
using namespace sf;

typedef Event::EventType ActionType;

struct InputTypeData
{
	ActionType type;
	int key;
};

struct ActionData
{
	string name;
	vector<InputTypeData> keys;
	function<void()> callback;

public:
	ActionData();
	template<class Class, typename RType = void, typename... Args>
	ActionData(const string& _name, Class* _owner, RType(Class::* _callback)(Args...),
		const InputTypeData& _primaryKey, const InputTypeData& _secondaryKey = InputTypeData(), Args... _args)
	{
		name = _name;
		keys.push_back(_primaryKey);
		keys.push_back(_secondaryKey);
		callback = [_owner, _callback, _args...]() {
			return (_owner->*_callback)(_args...);
		};
	}

	bool operator!() const
	{
		return name == "Unknown";
	}
};

class Action : public IManagable<string>
{
	ActionData data;
	string actionMap;

public:
	ActionData GetData() const
	{
		return data;
	}

public:
	Action(const ActionData& _data, const string& _actionMap);

public:
	virtual void Register() override;

public:
	bool ContainsKey(const int _keyToFind) const;
	bool IsEmpty() const;
	void Execute();
};