#include "Slime.h"
#include "Macro.h"

#include "DamageComponent.h"
#include "LifeComponent.h"
#include "AnimationComponent.h"

#define SOUND_ATTACK_SLIME "Assets/Sounds/en_slime_attack.ogg"
#define SOUND_HIT_SLIME "Assets/Sounds/en_slime_hit.ogg"
#define SOUND_DEATH_SLIME "Assets/Sounds/en_slime_death.ogg"

Slime::Slime(const float _maxHp, const float _maxDammage, const Vector2f& _position, const string& _path, const int _droppedCoins) : Enemy(_maxHp, _maxDammage, STRING_ID("Slime"), _path, _position, _droppedCoins)
{
	slimeType = S_NONE;
	components.push_back(new AnimationComponent(this, {
		AnimationData("Idle", Vector2f(26, 26), 0, 3, 0.1f, false),
	}, "Idle", visuals));

	attackSound = SOUND_ATTACK_SLIME;
	hitSound = SOUND_HIT_SLIME;
	deathSound = SOUND_DEATH_SLIME;
}

void Slime::SetNextDirection()
{
	if (indexPatern >= directionsPatern.size() || indexPatern < 0)
	{
		indexPatern = 0;
	}
	GetComponent<MovementComponent>()->SetDirection(directionsPatern[indexPatern]);
	indexPatern++;
}

void Slime::SelectDirection()
{
	if (GetComponent<MovementComponent>()->GetCanMove())
	{
		SetNextDirection();
	}
}