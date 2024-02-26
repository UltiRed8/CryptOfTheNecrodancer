#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "IManagable.h"

using namespace sf;
using namespace std;

class InputData : public IManagable<string>
{
	Event::EventType actionType;
	vector<int> keys;
	function<void()> callback;

public:
	Event::EventType GetActionType() const
	{
		return actionType;
	}
	bool ContainsKey(const int _keyToFind) const
	{
		if (keys.size() == 0) return true;
		for (const int _key : keys)
		{
			if (_key == _keyToFind)
			{
				return true;
			}
		}
		return false;
	}
	bool IsEmpty() const
	{
		return (keys.size() == 0);
	}

public:
	template <class Class, typename... Args>
	InputData(const string& _id, const Event::EventType& _actionType,
		const vector<int> _keys, Class* _owner, void (Class::* _callback)(Args...), const Args&... _args)
		: IManagable(_id)
	{
		Register();
		actionType = _actionType;
		keys = _keys;
		callback = [_owner, _callback, _args...]()
		{
			(_owner->*_callback)(_args...);
		};
	}

private:
	virtual void Register() override;

public:
	void DoAction() const;
};