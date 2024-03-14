#include "Chest.h"
#include "Map.h"

#define PATH_CHEST "Entities/chest.png"

Chest::Chest(const Vector2f& _pos) : Placeable(STRING_ID("Chest"), PATH_CHEST, _pos)
{
	type = ET_CHEST;
	zIndex = 2;
}

Chest::~Chest()
{
	//EraseElement(Map::GetInstance().GetGenerator()->GetOthers(),_entity);
}

Item* Chest::CreateRandomItem()
{
	SlotType _type = (SlotType)Random(6, 0);

	vector<vector<int>> _items = {
		{PT_PICKAXE, PT_SHOVEL},
		{WT_BROADSWORD, WT_DAGGER, WT_SPEAR, WT_STAFF},
		{AT_BODY_CHAINMAIL, AT_BODY_LEATHERARMOR, AT_BODY_HEAVYPLATE, AT_BODY_PLATEMAIL},
		{AT_HEAD_HELMET, AT_HEAD_MINERSCAP,},
		{AT_FEET_BOOTSOFSTRENGTH, AT_FEET_HARGREAVES},
		{},
		{},
	};

	/*enum WeaponType
	{
		WT_BROADSWORD, WT_DAGGER, WT_SPEAR, WT_STAFF
	};
	//
	enum PickaxeType
	{
		PT_PICKAXE, PT_SHOVEL
	};
	//
	enum ArmorType
	{
		AT_HEAD_HELMET, AT_HEAD_MINERSCAP,
		AT_BODY_CHAINMAIL, AT_BODY_LEATHERARMOR, AT_BODY_HEAVYPLATE, AT_BODY_PLATEMAIL,
		AT_FEET_BOOTSOFSTRENGTH, AT_FEET_HARGREAVES
	};
	//
	enum PickableType
	{
		PIT_COIN, PIT_DIAMOND, PIT_HEART
	};*/

	const vector<int>& _path = _items[_type];
	if (_path.empty())
	{
		return nullptr;
	}
	const int _itemType = _path[Random(_path.size() - 1, 0)];

	vector<function<Item* ()>> _actions = {
		[&]() { return new Pickaxe((PickaxeType)_itemType,STRING_ID("Pickaxe"),GetPosition()); },
		[&]() { return new Weapon((WeaponType)_itemType,STRING_ID("Weapon"),GetPosition()); },
		[&]() { return new Armor((ArmorType)_itemType,STRING_ID("Armor"),GetPosition()); },
		[&]() { return new Armor((ArmorType)_itemType,STRING_ID("Armor"),GetPosition()); },
		[&]() { return new Armor((ArmorType)_itemType,STRING_ID("Armor"),GetPosition()); },
		nullptr,
		nullptr,
		nullptr,
	};

	if (!_actions[_type])
	{
		return nullptr;
	}
	Item* _item = _actions[_type]();
	Map::GetInstance().AddItem(_item);
	return _item;
}

void Chest::Open()
{
	CreateRandomItem();
	Destroy();
}
