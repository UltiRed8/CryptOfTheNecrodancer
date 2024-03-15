#include "ShopStand.h"
#include "Macro.h"
#include "Player.h"
#include "EntityManager.h"
#include "Map.h"

#define PATH_SAVE "Assets/Saved/PurchasedItems.txt"

ShopStand::ShopStand(const Vector2f& _position, const StandType& _type)
{
	position = _position;
	standType = _type;
	ownedItems = vector<int>();
	UpdateOwnedItems();
	UpdateAllItems();
	SpawnItems();
}

ShopStand::ShopStand(const vector<Vector2f>& _positions, const StandType& _type)
{
	standType = _type;
	ownedItems = vector<int>();
	UpdateOwnedItems();
	UpdateAllItems();
	for (const Vector2f& _position : _positions)
	{
		position = _position;
		SpawnItems();
	}
}

void ShopStand::UpdateOwnedItems()
{
	ifstream _in = ifstream(PATH_SAVE);
	string _line;
	while (getline(_in, _line))
	{
		ownedItems.push_back(stoi(_line));
	}
}

void ShopStand::UpdateAllItems()
{
	const int _list[] = {
		WT_SIZE,
		PT_SIZE,
		AT_SIZE,
		CT_SIZE,
	};

	for (int _size : _list)
	{
		for (int _i = 0; _i < _size; _i++)
		{
			allItems.push_back(_i);
		}
	}

	EraseElements(allItems, ownedItems);
}

void ShopStand::SpawnItems()
{
	function<void()> _actions[] = {
		[this]() { SpawnLobbyItems(); },
		[this]() { SpawnHeartsItems(); },
		[this]() { SpawnShopKeeperItems(); },
	};

	_actions[standType]();
}

vector<Vector2f> ShopStand::GetSpawnPositions()
{
	vector<Vector2f> _positions;
	_positions.push_back(position + Vector2f(-1.0f, 0.0f) * TILE_SIZE);
	_positions.push_back(position);
	_positions.push_back(position + Vector2f(1.0f, 0.0f) * TILE_SIZE);
	return _positions;
}

void ShopStand::SpawnLobbyItems()
{
	for (const Vector2f& _spawnPos : GetSpawnPositions())
	{
		if (allItems.empty()) return;

		const int _itemID = allItems[Random((const int)allItems.size() - 1, 0)];

		function<Item* (const int _id, const Vector2f& _position)> _actions[] = {
			[this](const int _id, const Vector2f& _position) { return new Weapon((WeaponType)_id, STRING_ID("Weapon"), _position); }, // weapon
			[this](const int _id, const Vector2f& _position) { return new Pickaxe((PickaxeType)(_id - 100), STRING_ID("Pickaxe"), _position); }, // pickaxe
			[this](const int _id, const Vector2f& _position) { return new Armor((ArmorType)(_id - 200), STRING_ID("Armor"), _position); }, // armor
			[this](const int _id, const Vector2f& _position) { return new Consomable((ConsomableType)(_id - 300), STRING_ID("Consomable"), _position); }, // consomable
		};

		const function<Item* (const int _id, const Vector2f& _position)>& _callback = _actions[(int)floor(_itemID / 100.0f)];

		if (_callback)
		{
			Map::GetInstance().AddItem(_callback(_itemID, _spawnPos));
		}

		EraseElement(allItems, _itemID);
	}
}

void ShopStand::SpawnHeartsItems()
{
	int _currentHearts = (int)(EntityManager::GetInstance().Get("Player")->GetComponent<LifeComponent>()->GetMaxHealth() / 100.0f);

	for (const Vector2f& _spawnPos : GetSpawnPositions())
	{
		if (_currentHearts >= 6) return;
		_currentHearts++;
		Map::GetInstance().AddItem(new Pickable(PIT_HEART, STRING_ID("Heart"), _spawnPos));
	}
}

void ShopStand::SpawnShopKeeperItems()
{
	for (const Vector2f& _spawnPos : GetSpawnPositions())
	{
		if (ownedItems.empty()) return;

		const int _itemID = ownedItems[Random((const int)ownedItems.size() - 1, 0)];

		function<Item*(const int _id, const Vector2f& _position)> _actions[] = {
			[this](const int _id, const Vector2f& _position) { return new Weapon((WeaponType)_id, STRING_ID("Weapon"), _position); }, // weapon
			[this](const int _id, const Vector2f& _position) { return new Pickaxe((PickaxeType)(_id - 100), STRING_ID("Pickaxe"), _position); }, // pickaxe
			[this](const int _id, const Vector2f& _position) { return new Armor((ArmorType)(_id - 200), STRING_ID("Armor"), _position); }, // armor
			[this](const int _id, const Vector2f& _position) { return new Consomable((ConsomableType)(_id - 300), STRING_ID("Consomable"), _position); }, // consomable
		};

		const function<Item*(const int _id, const Vector2f& _position)>& _callback = _actions[(int)floor(_itemID / 100.0f)];

		if (_callback)
		{
			Map::GetInstance().AddItem(_callback(_itemID, _spawnPos));
		}

		EraseElement(ownedItems, _itemID);
	}
}