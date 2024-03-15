#include "Chest.h"
#include "Map.h"

#define PATH_CHEST "Entities/chest.png"

#define SOUND_CHEST_OPEN "Assets/Sounds/obj_chest_open.ogg"
#define SOUND_CHEST_APPEAR "Assets/Sounds/obj_chest_appear_ST.ogg"

Chest::Chest(const Vector2f& _pos) : Placeable(STRING_ID("Chest"), PATH_CHEST, _pos)
{
	type = ET_CHEST;
	zIndex = 2;
}

Item* Chest::CreateRandomItem()
{
	const int _type = Random(6, 0);

	vector<vector<int>> _items = {
		{PT_PICKAXE, PT_SHOVEL, PT_SHOVEL_COURAGE, PT_SHOVEL_CRYSTAL, PT_SHOVEL_TITANIUM,},
		{WT_WEAPONAXE, WT_WEAPONAXE_TITANIUM, WT_BROADSWORD, WT_BROADSWORD_TITANIUM, WT_CAT,
		 WT_CAT_TITANIUM, WT_CUTLASS, WT_CUTLASS_TITANIUM, WT_DAGGER, WT_DAGGER_JEWELED,
		 WT_DAGGER_TITANIUM, WT_FLAIL, WT_FLAIL_TITANIUM, WT_HARP, WT_HARP_TITANIUM,
		 WT_LONGSWORD, WT_LONGSWORD_TITANIUM, WT_RAPIER, WT_RAPIER_TITANIUM, WT_SPEAR, WT_SPEAR_TITANIUM,
		 WT_STAFF, WT_STAFF_TITANIUM, WT_WARHAMMER, WT_WARHAMMER_TITANIUM, WT_WHIP, WT_WHIP_TITANIUM,},
		{AT_BODY_CHAINMAIL, AT_BODY_LEATHERARMOR, AT_BODY_HEAVYPLATE, AT_BODY_PLATEMAIL},
		{AT_HEAD_HELMET, AT_HEAD_MINERSCAP,},
		{AT_FEET_BOOTSOFSTRENGTH, AT_FEET_HARGREAVES},
		{ CT_APPLE, CT_CARROT, CT_CHEESE, CT_COOKIE, CT_DRUMSTICK, CT_HAM },
	};

	if (_type == 6)
	{
		Item* _item = new BombItem(GetPosition(), false);
		Map::GetInstance().AddItem(_item);
		return _item;
	}

	const vector<int>& _path = _items[_type];
	
	const int _itemType = _path[Random((const int)_path.size() - 1, 0)];

	vector<function<Item* ()>> _actions = {
		[&]() { return new Pickaxe((PickaxeType)_itemType,STRING_ID("Pickaxe"),GetPosition(), false, false); },
		[&]() { return new Weapon((WeaponType)_itemType,STRING_ID("Weapon"),GetPosition(), false, false); },
		[&]() { return new Armor((ArmorType)_itemType,STRING_ID("Armor"),GetPosition(), false, false); },
		[&]() { return new Armor((ArmorType)_itemType,STRING_ID("Armor"),GetPosition(), false, false); },
		[&]() { return new Armor((ArmorType)_itemType,STRING_ID("Armor"),GetPosition(), false, false); },
		[&]() { return new Consomable((ConsomableType)_itemType,STRING_ID("Food"),GetPosition(), false); },
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
	SoundManager::GetInstance().Play(SOUND_CHEST_OPEN);
	SoundManager::GetInstance().Play(SOUND_CHEST_APPEAR);
	Destroy();
}
