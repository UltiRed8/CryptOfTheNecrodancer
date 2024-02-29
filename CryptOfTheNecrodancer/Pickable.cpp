#include "Pickable.h"
#include "EntityManager.h"
#include "SoundManager.h"

#define SOUND_COIN_PICKED_UP "Assets/Sounds/sfx_pickup_gold_01.ogg"
#define SOUND_DIAMOND_PICKED_UP "Assets/Sounds/sfx_pickup_diamond.ogg"
#define PATH_COIN "Coins.png"
#define PATH_DIAMOND "Diamond.png"

Pickable::Pickable(const int _amount, const PickableType& _type, const string& _id, const Vector2f& _pos, const string& _path) : Placable(_id,_path, _pos)
{
	amount = _amount;
	pickableType = _type;

	zIndex = 1;

	if (_type != PT_CUSTOM)
	{
		SetTexture(_type == PT_COIN ? PATH_COIN :  PATH_DIAMOND);
	}
	type = ET_PICKABLE;
}

void Pickable::PickUp()
{
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	string _path;
	if (type == PT_DIAMOND)
	{
		_player->GetRessources()->AddDiamonds(amount);
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
