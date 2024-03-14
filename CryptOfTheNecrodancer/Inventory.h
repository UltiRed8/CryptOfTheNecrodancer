#pragma once
#include <iostream>
#include "Menu.h"
#include "Macro.h"
#include "Items.h"

using namespace std;

class Item;

struct Slot : public UIImage
{
	SlotType type;
	UIImage* item;
	bool isVisible;
	Item* currentItem;

public:
	Slot(const SlotType& _type, const string& _path, Menu* _owner);
	~Slot();

public:
	void SetItemPosition(const Vector2f& _position)
	{
		const Vector2f& _itemSize = item->GetShape()->getGlobalBounds().getSize();
		item->GetShape()->setPosition(_position + _itemSize / 4.f);
	}
	Item* GetItem() const
	{
		return currentItem;
	}
	void SetItem(Item* _item)
	{
		if (!_item)
		{
			isVisible = false;
		}
		currentItem = _item;
	}
	void SetPosition(const Vector2f& _position)
	{
		shape->setPosition(_position);
	}
	void SetVisible()
	{
		isVisible = true;
	}
	SlotType GetType() const
	{
		return type;
	}
	virtual vector<Drawable*> GetDrawables() override;
};

class Inventory : public Menu
{
	vector<Slot*> others;
	vector<Slot*> usables;
	vector<Slot*> foods;

public:
	Inventory();
	~Inventory();

public:
	vector<Slot*> GetSlots()
	{
		vector<Slot*> _all;
		_all.insert(_all.end(), others.begin(), others.end());
		_all.insert(_all.end(), usables.begin(), usables.end());
		_all.insert(_all.end(), foods.begin(), foods.end());
		return _all;
	}
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