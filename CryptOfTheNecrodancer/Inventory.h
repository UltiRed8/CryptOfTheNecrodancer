#pragma once
#include <iostream>
#include "Menu.h"
#include "Macro.h"
#include "Item.h"
#include "Items.h"

using namespace std;

struct Slot : public UIImage
{
	SlotType type;
	UIImage* item;
	bool isVisible;

public:
	Slot(const SlotType& _type, const string& _path, Menu* _owner) : UIImage(STRING_ID(to_string(type) + "_slot"), Vector2f(0.0f, 0.0f), Vector2f(30.0f, 33.0f) * 3.0f, _path)
	{
		type = _type;
		item = new UIImage(STRING_ID("item_" + to_string(type) + "_slot"), Vector2f(0.0f, 0.0f), Vector2f(30.0f, 33.0f) * 3.0f, "");
		isVisible = false;
		item->SetOwner(_owner);
		item->Register();
	}

public:
	void SetPosition(const Vector2f& _position)
	{
		shape->setPosition(_position);
	}
	void Toggle()
	{
		isVisible = !isVisible;
	}
};

class Inventory : public Menu
{
	vector<Slot*> others;
	vector<Slot*> usables;
	vector<Slot*> foods;

public :
	Inventory();
	~Inventory();

public:
	Slot* GetSlot(const SlotType& _type)
	{
		vector<Slot*> _all;
		_all.insert(_all.end(), others.begin(), others.end());
		_all.insert(_all.end(), usables.begin(), usables.end());
		_all.insert(_all.end(), foods.begin(), foods.end());

		for (Slot* _slot : _all)
		{
			if (_slot->type == _type)
			{
				return _slot;
			}
		}
		return nullptr;
	}

public:
	virtual vector<Drawable*> GetDrawables() override;
};