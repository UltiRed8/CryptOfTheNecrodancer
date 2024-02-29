#pragma once

#include <map>
#include <vector>
#include <iostream>

#include "Debug.h"

using namespace std;

template<typename Key, typename Value>
class IManager
{
	Value* current;
protected:
	map<Key, Value*> allValues;
	vector<Value*> unregisteredValues;

public:
	~IManager()
	{
		for (auto _pair : allValues)
		{
			delete _pair.second;
		}
		for (Value* _value : unregisteredValues)
		{
			delete _value;
		}
	}

public:
	void Add(const Key& _key, Value* _value)
	{
		if (Exist(_value))
		{
			unregisteredValues.push_back(_value);
			return;
		}
		_value->SetID(_key);
		allValues[_key] = _value;
		current = _value;
	}
	void Remove(Value* _value)
	{
		if (!Exist(_value)) return;
		allValues[_value->GetID()]->Destroy();
	}
	void Remove(const Key& _key)
	{
		if (!Exist(_key)) return;
		allValues[_key]->Destroy();
	}
	void GarbageCollector()
	{
		vector<Key> _keysToRemove;
		for (const auto& _pair : allValues)
		{
			if (_pair.second->IsToRemove())
			{
				_keysToRemove.push_back(_pair.first);
			}
		}

		for (const Key& _key : _keysToRemove)
		{
			Value* _value = allValues[_key];
			Debug(_key, _value);
			allValues.erase(_key);
			delete _value;
			_value = nullptr;
		}

		for (Value* _value : unregisteredValues)
		{
			Debug(_value);
			delete _value;
			_value = nullptr;
		}
		unregisteredValues.clear();
	}
	Value* GetCurrent() const
	{
		return current;
	}
	Value* Get(const Key& _key)
	{
		if (!Exist(_key)) return nullptr;
		current = allValues[_key];
		return allValues[_key];
	}
	map<Key, Value*> GetAll() const
	{
		return allValues;
	}
	vector<Value*> GetAllValues() const
	{
		vector<Value*> _values;
		for (const auto& _pair : allValues)
		{
			_values.push_back(_pair.second);
		}
		return _values;
	}
	bool Exist(const Key& _key) const
	{
		return allValues.find(_key) != allValues.end();
	}
	bool Exist(Value* _value) const
	{
		return Exist(_value->GetID());
	}
	int Count() const
	{
		return static_cast<const int>(allValues.size());
	}
};