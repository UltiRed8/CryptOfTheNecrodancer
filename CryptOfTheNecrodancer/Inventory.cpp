#include "Inventory.h"
#include "MenuManager.h"
#include "SoundManager.h"
#include "Macro.h"
#include "Item.h"
#include "Map.h"

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
	Menu* _menu = MenuManager::GetInstance().Get("Inventory");
	others.push_back(new Slot(ST_SHOVEL, I_SHOVEL, _menu));
	others.push_back(new Slot(ST_ATTACK, I_ATTACK, _menu));
	others.push_back(new Slot(ST_BODY, I_BODY, _menu));
	others.push_back(new Slot(ST_HEAD, I_HEAD, _menu));
	others.push_back(new Slot(ST_FEET, I_FEET, _menu));
	usables.push_back(new Slot(ST_BOMB, I_BOMB, _menu));

	foods.push_back(new Slot(ST_FOOD_TOP, I_ITEM, _menu));
	foods.push_back(new Slot(ST_FOOD_DOWN, I_ITEM, _menu));
}

Inventory::~Inventory()
{
	for (Slot* _slot : GetSlots())
	{
		_slot->Destroy();
	}
}

void Inventory::Reset()
{
	for (Slot* _slot : GetSlots())
	{
		_slot->Destroy();
	}
	others.clear();
	usables.clear();
	foods.clear();
	Menu* _menu = MenuManager::GetInstance().Get("Inventory");
	others.push_back(new Slot(ST_SHOVEL, I_SHOVEL, _menu));
	others.push_back(new Slot(ST_ATTACK, I_ATTACK, _menu));
	others.push_back(new Slot(ST_BODY, I_BODY, _menu));
	others.push_back(new Slot(ST_HEAD, I_HEAD, _menu));
	others.push_back(new Slot(ST_FEET, I_FEET, _menu));
	usables.push_back(new Slot(ST_BOMB, I_BOMB, _menu));

	foods.push_back(new Slot(ST_FOOD_TOP, I_ITEM, _menu));
	foods.push_back(new Slot(ST_FOOD_DOWN, I_ITEM, _menu));
}

vector<Drawable*> Inventory::GetDrawables()
{
	vector<Drawable*> _drawables;
	Vector2f _position = Vector2f(10.0f, 10.0f);

	for (Slot* _slot : others)
	{
		if (!_slot->isVisible) continue;
		_slot->SetPosition(_position);
		_slot->SetItemPosition(_position);
		vector<Drawable*> _slotDrawables = _slot->GetDrawables();
		_drawables.insert(_drawables.end(), _slotDrawables.begin(), _slotDrawables.end());
		_position.x += 110.0f;
	}

	_position = Vector2f(10.0f, 140.0f);
	for (Slot* _slot : usables)
	{
		if (!_slot->isVisible) continue;
		_slot->SetPosition(_position);
		_slot->SetItemPosition(_position);
		vector<Drawable*> _slotDrawables = _slot->GetDrawables();
		_drawables.insert(_drawables.end(), _slotDrawables.begin(), _slotDrawables.end());
		_position.y += 110.0f;
	}

	_position = Vector2f(SCREEN_WIDTH - 10.0f - 90.0f, 140.0f);
	for (Slot* _slot : foods)
	{
		if (!_slot->isVisible) continue;
		_slot->SetPosition(_position);
		_slot->SetItemPosition(_position);
		vector<Drawable*> _slotDrawables = _slot->GetDrawables();
		_drawables.insert(_drawables.end(), _slotDrawables.begin(), _slotDrawables.end());
		_position.y += 110.0f;
	}

	return _drawables;
}

Slot::Slot(const SlotType& _type, const string& _path, Menu* _owner) : UIImage(STRING_ID(to_string(type) + "_slot"), Vector2f(0.0f, 0.0f), Vector2f(30.0f, 33.0f) * 3.0f, _path)
{
	type = _type;
	const string& _rPath = _type == ST_SHOVEL ? PATH_SHOVEL : _type == ST_ATTACK ? PATH_DAGGER : "";
	item = new UIImage(STRING_ID("item_" + to_string(type) + "_slot"), Vector2f(0.0f, 0.0f), Vector2f(30.0f, 33.0f) * 2.0f, "");
	currentItem = _type == ST_SHOVEL ? (Item*) new Pickaxe(PT_SHOVEL, STRING_ID("Pickaxe"), {}, true)
		: _type == ST_ATTACK ? (Item*) new Weapon(WT_DAGGER, STRING_ID("Dagger"), {}, true)
		: _type == ST_BOMB ? (Item*) new BombItem({})
		: nullptr;
	
	isVisible = _type == ST_SHOVEL ? true : _type == ST_ATTACK ? true : _type == ST_BOMB ? true : false;
	item->SetOwner(_owner);
	item->Register();
}

Slot::~Slot()
{
	if (currentItem)
	{
		delete currentItem;
	}
}

vector<Drawable*> Slot::GetDrawables()
{
	vector<Drawable*> _drawables;

	_drawables.push_back(shape);
	if (currentItem)
	{
		item->GetShape()->setTexture(currentItem->GetVisuals()->getTexture());
	}
	_drawables.push_back(item->GetShape());

	return _drawables;
}
