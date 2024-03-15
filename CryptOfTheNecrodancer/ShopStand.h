#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

using namespace std;
using namespace sf;

enum StandType
{
	STT_LOBBY, STT_HEARTS, STT_SHOPKEEPER
};

class ShopStand
{
	Vector2f position;
	StandType standType;
	vector<int> ownedItems;
	vector<int> allItems;

public:
	ShopStand(const Vector2f& _position, const StandType& _type);
	ShopStand(const vector<Vector2f>& _positions, const StandType& _type);

private:
	void UpdateAllItems();
	void UpdateOwnedItems();
	void SpawnItems();
	vector<Vector2f> GetSpawnPositions();
	void SpawnLobbyItems();
	void SpawnHeartsItems();
	void SpawnShopKeeperItems();
};