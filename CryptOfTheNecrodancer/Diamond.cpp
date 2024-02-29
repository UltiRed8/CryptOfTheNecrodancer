#include "Diamond.h"
#include "TextureManager.h"
#include "Player.h"
#include "SoundManager.h"
#include "EntityManager.h"

Diamond::Diamond(const string& _id, const Vector2f& _position) : Tile(PATH_DIAMOND, _position, ET_DIAMOND)
{
	zIndex = 1;
	
	TextureManager::GetInstance().Load(shape, PATH_DIAMOND);
}

void Diamond::PickUpDiamond()
{
	Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
	_player->GetRessources()->AddDiamonds();
	Destroy();
	SoundManager::GetInstance().Play(PATH_DIAMOND_PICKED_UP);
}