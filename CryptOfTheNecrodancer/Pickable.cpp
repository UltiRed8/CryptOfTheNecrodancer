#include "Pickable.h"
#include "EntityManager.h"
#include "SoundManager.h"
#include "Macro.h"

#define SOUND_COIN_PICKED_UP "Assets/Sounds/sfx_pickup_gold_01.ogg"
#define SOUND_DIAMOND_PICKED_UP "Assets/Sounds/sfx_pickup_diamond.ogg"
#define PATH_COIN "Entities/Coins.png"
#define PATH_DIAMOND "UI/Diamond.png"

Pickable::Pickable(const int _amount, const PickableType& _type, const string& _id, const Vector2f& _pos, const string& _path) : Placeable(_id,_path, _pos)
{
	amount = _amount;
	pickableType = _type;

	zIndex = 2;
	if (_type != PT_CUSTOM)
	{
		if (_type == PT_COIN)
		{
			TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_COIN, Random(3, 0), Vector2i(24, 24));
		}
		else if (_type == PT_DIAMOND)
		{
			SetTexture(PATH_DIAMOND);
			shape->setPosition(Vector2f(_pos.x + TILE_SIZE.x / 4.0f, _pos.y) + Vector2f(0.0f, -0.5f) * TILE_SIZE / 2.0f);
			shape->setScale({ 0.5f, 0.5f });
		}
	}

	type = ET_PICKABLE;
}

void Pickable::PickUp()
{
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	string _path;
	if (pickableType == PT_DIAMOND)
	{
		_player->GetRessources()->AddDiamonds();
		_path = SOUND_DIAMOND_PICKED_UP;
	}
	else 
	{
		_player->GetRessources()->AddMoney(amount);
		_path = SOUND_COIN_PICKED_UP;
	}
	SoundManager::GetInstance().Play(_path);
	this->Destroy();
}
