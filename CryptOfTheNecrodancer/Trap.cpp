#include "Trap.h"
#include "Macro.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "Player.h"
#include "MusicManager.h"
#include "Bomb.h"

Trap::Trap(const string& _path, const Vector2f& _position, const TrapType& _trapType) : Entity(STRING_ID("Trap"), _path, _position)
{
	type = ET_TRAP;
	trapType = _trapType;
	beatBeforeEndOfEffect = INFINITY;
	TextureManager::GetInstance().Load(shape, _path);
}

void Trap::ExecuteTrap(Entity* _entity)
{
	//TODO
	if (trapType == TR_CONFUSION)
	{
		TextureManager::GetInstance().Load(shape, PATH_CONFUSE_TRAP_PRESSED);
		// GET PLAYER AND SET CONFUSE
		if (_entity->GetType() == ET_PLAYER)
		{
			Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"));
			_player->SetIsConfuse(true);
		}
		// TODO TEMPS DE CONFUSION
		// 8 beats

	}
	if (trapType == TR_FAST)
	{
		TextureManager::GetInstance().Load(shape, PATH_FAST_TRAP_PRESSED);
		MusicManager::GetInstance().SpeedUp();
	}
	if (trapType == TR_SLOW)
	{
		TextureManager::GetInstance().Load(shape, PATH_SLOW_TRAP_PRESSED);
		MusicManager::GetInstance().SpeedDown();
	}
	if (trapType == TR_BOMB)
	{
		new Bomb(GetPosition(), true); // Pos and isActive
	}


}

void Trap::Update(const float& _deltaTime)
{
	// ADD DELTA TIME TO THE TIMER "operationTime"
	// operationTime += _deltaTime;
	beatBeforeEndOfEffect--;

	/*if (beatBeforeEndOfEffect == 0)
	{

	}*/
}


