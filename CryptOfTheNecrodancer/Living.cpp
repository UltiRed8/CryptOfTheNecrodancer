#include "Living.h"
#include "LifeComponent.h"
#include "DamageComponent.h"
#include "MovementComponent.h"
#include "SoundManager.h"
#include "Pickable.h"
#include "Map.h"

#define SOUND_DEATH "Assets/Sounds/en_general_death.ogg"
#define SOUND_HIT "Assets/Sounds/en_general_hit.ogg"

#define SOUND_HIT_PLAYER "Assets/Sounds/sfx_player_hit_ST.ogg"

Living::Living(const float _maxHp, const float _maxDammage, const string& _path, const string& _id, const Vector2f& _pos, const bool _shouldResetDirection) : Entity(_id, _path, _pos)
{
	components.push_back(new MovementComponent(this, 1, _shouldResetDirection));
	components.push_back(new LifeComponent(this, [&]() {
		SoundManager::GetInstance().Play(SOUND_DEATH);
		this->DieEvent();
	}, false, _maxHp));
	components.push_back(new DamageComponent(this, _maxDammage));
}

Living::Living(const string& _path, const string& _id, const Vector2f& _pos) : Entity(_id, _path, _pos)
{
	
}

void Living::Hit()
{
	if ((Player*)this)
	{
		SoundManager::GetInstance().Play(SOUND_HIT_PLAYER);
	}
	else
	{
		SoundManager::GetInstance().Play(SOUND_HIT);
	}
	SoundManager::GetInstance().Play(hitSound);
}