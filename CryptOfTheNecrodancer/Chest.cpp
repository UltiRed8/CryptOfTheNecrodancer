#include "Chest.h"
#include "Map.h"

#define PATH_CHEST "Entities/chest.png"

#define SOUND_CHEST_OPEN "Assets/Sounds/obj_chest_open.ogg"
#define SOUND_CHEST_APPEAR "Assets/Sounds/obj_chest_appear_ST.ogg"

#define PATH_SAVE "Assets/Saved/PurchasedItems.txt"

Chest::Chest(const Vector2f& _pos) : Placeable(STRING_ID("Chest"), PATH_CHEST, _pos)
{
	type = ET_CHEST;
	zIndex = 2;
	ownedItems = vector<int>();
	UpdateOwnedItems();
}

void Chest::UpdateOwnedItems()
{
	ifstream _in = ifstream(PATH_SAVE);
	string _line;
	while (getline(_in, _line))
	{
		ownedItems.push_back(stoi(_line));
	}
}

Item* Chest::CreateRandomItem()
{
	if (ownedItems.empty()) return nullptr;

	const int _type = Random(6, 0);

	if (_type == 6)
	{
		Item* _item = new BombItem(GetPosition(), false);
		Map::GetInstance().AddItem(_item);
		return _item;
	}

	const int _itemID = ownedItems[Random((const int)ownedItems.size() - 1, 0)];

	function<Item* (const int _id, const Vector2f& _position)> _actions[] = {
		[this](const int _id, const Vector2f& _position) { return new Weapon((WeaponType)_id, STRING_ID("Weapon"), _position, false, false); }, // weapon
		[this](const int _id, const Vector2f& _position) { return new Pickaxe((PickaxeType)(_id - 100), STRING_ID("Pickaxe"), _position, false, false); }, // pickaxe
		[this](const int _id, const Vector2f& _position) { return new Armor((ArmorType)(_id - 200), STRING_ID("Armor"), _position, false, false); }, // armor
		[this](const int _id, const Vector2f& _position) { return new Consomable((ConsomableType)(_id - 300), STRING_ID("Consomable"), _position, false); }, // consomable
	};

	const function<Item* (const int _id, const Vector2f& _position)>& _callback = _actions[(int)floor(_itemID / 100.0f)];

	if (_callback)
	{
		Item* _item = _callback(_itemID, GetPosition());
		Map::GetInstance().AddItem(_item);
		return _item;
	}
	return nullptr;
}

void Chest::Open()
{
	CreateRandomItem();
	SoundManager::GetInstance().Play(SOUND_CHEST_OPEN);
	SoundManager::GetInstance().Play(SOUND_CHEST_APPEAR);
	Destroy();
}
