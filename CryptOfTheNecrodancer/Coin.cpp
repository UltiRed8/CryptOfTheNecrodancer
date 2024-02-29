#include "Coin.h"
#include "Macro.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include "Player.h"
#include "SoundManager.h"

#define PATH_COIN_SOUND "Assets/Sounds/sfx_pickup_gold_01.ogg"

Coin::Coin(const int _coins, const string& _id, const Vector2f& _position)
	: Tile("", _position, ET_COIN)
{
	coins = _coins;
	zIndex = 1;

	TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_COIN, Random(3, 0), Vector2i(24, 24));
}

void Coin::PickUpCoin()
{
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	_player->GetRessources()->AddMoney(coins);
	Destroy();
	SoundManager::GetInstance().Play(PATH_COIN_SOUND);
}
