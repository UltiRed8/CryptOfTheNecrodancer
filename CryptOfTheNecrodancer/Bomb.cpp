#include "Bomb.h"
#include "EntityManager.h"
#include "Macro.h"



#define BOMB_PATH "Entities/Bomb.png"

Bomb::Bomb(const Vector2f& pos, const bool _isActive) : Placable(STRING_ID("Bomb"), BOMB_PATH, pos)
{
	isActive = _isActive;
	cooldown = 4;
}

void Bomb::Explode()
{
	// TODO ANIMATION EXPLOSION
	
	map<string, Entity*> _allEntities = EntityManager::GetInstance().GetAll();

	for (const auto& _entity : _allEntities)
	{
		//if (_entity.second->GetPosition())
		//{

		//}
		_entity.second->Destroy();
	}

}

void Bomb::Update()
{
	cooldown--;

	if (cooldown == 0)
	{
		Explode();
		Destroy();
	}
}
