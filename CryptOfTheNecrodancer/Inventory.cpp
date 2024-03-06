#include "Inventory.h"
#include "MenuManager.h"
#include "Macro.h"

//HUD FRAME
#define I_SHOVEL "UI/hud_shovel.png"
#define I_ATTACK "UI/hud_attack.png"
#define I_BODY "UI/hud_body.png"
#define I_HEAD "UI/hud_head.png"
#define I_FEET "UI/hud_feet.png"
#define I_THROW "UI/hud_throw.png"
#define I_BOMB "UI/hud_bomb.png"
#define I_ITEM "UI/hud_item.png"

#define SHOVEL "Items/Shovel/Shovel.png"
#define PICKAXE "Items/Shovel/Pickaxe.png"

Inventory::Inventory() : Menu("Inventory", {}, 0, false)
{
	others.push_back(new Slot(ST_SHOVEL, I_SHOVEL, this));
	others.push_back(new Slot(ST_ATTACK, I_ATTACK, this));
	others.push_back(new Slot(ST_BODY, I_BODY, this));
	others.push_back(new Slot(ST_HEAD, I_HEAD, this));
	others.push_back(new Slot(ST_FEET, I_FEET, this));

	usables.push_back(new Slot(ST_THROW, I_THROW, this));
	usables.push_back(new Slot(ST_BOMB, I_BOMB, this));

	foods.push_back(new Slot(ST_FOOD_TOP, I_ITEM, this));
	foods.push_back(new Slot(ST_FOOD_DOWN, I_ITEM, this));
}

Inventory::~Inventory()
{
	for (Slot* _slot : others)
	{
		delete _slot;
	}
	for (Slot* _slot : usables)
	{
		delete _slot;
	}
	for (Slot* _slot : foods)
	{
		delete _slot;
	}
}

vector<Drawable*> Inventory::GetDrawables()
{
	vector<Drawable*> _drawables;
	Vector2f _position = Vector2f(10.0f, 10.0f);

	for (Slot* _slot : others)
	{
		if (!_slot->isVisible) continue;
		_slot->SetPosition(_position);
		vector<Drawable*> _slotDrawables = _slot->GetDrawables();
		_drawables.insert(_drawables.end(), _slotDrawables.begin(), _slotDrawables.end());
		_position.x += 110.0f;
	}

	_position = Vector2f(10.0f, 140.0f);
	for (Slot* _slot : usables)
	{
		if (!_slot->isVisible) continue;
		_slot->SetPosition(_position);
		vector<Drawable*> _slotDrawables = _slot->GetDrawables();
		_drawables.insert(_drawables.end(), _slotDrawables.begin(), _slotDrawables.end());
		_position.y += 110.0f;
	}

	_position = Vector2f(SCREEN_WIDTH - 10.0f - 90.0f, 140.0f);
	for (Slot* _slot : foods)
	{
		if (!_slot->isVisible) continue;
		_slot->SetPosition(_position);
		vector<Drawable*> _slotDrawables = _slot->GetDrawables();
		_drawables.insert(_drawables.end(), _slotDrawables.begin(), _slotDrawables.end());
		_position.y += 110.0f;
	}

	return _drawables;
}