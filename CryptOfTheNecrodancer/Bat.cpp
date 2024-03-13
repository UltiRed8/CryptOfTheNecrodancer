#include "Bat.h"
#include"Macro.h"
#include"MovementComponent.h"
#include"DamageComponent.h"
#include"LifeComponent.h"
#include"Macro.h"
#include "AnimationComponent.h"

#define PATH_BAT "Entities/BatSprite.png"

#define SOUND_ATTACK_BAT  "Assets/Sounds/en_bat_attack.ogg"
#define SOUND_HIT_BAT  "Assets/Sounds/en_bat_hit.ogg"
#define SOUND_DEATH_BAT  "Assets/Sounds/en_bat_death.ogg"

Bat::Bat(const Vector2f& _position) :Enemy(100.f,50.f,STRING_ID("Bat"), PATH_BAT, _position, 2)
{
	directionsPatern = {
		Vector2i(1,0),
		Vector2i(-1,0),
		Vector2i(0,1),
		Vector2i(0,-1),
	};

	components.push_back(new AnimationComponent(this, {
		AnimationData("Idle", Vector2f(24, 24), 0, 3, 0.1f, true),
	}, "Idle", visuals));

	cooldown = 2;

	attackSound = SOUND_ATTACK_BAT;
	hitSound = SOUND_HIT_BAT;
	deathSound = SOUND_DEATH_BAT;
}

void Bat::SelectDirection()
{
	int _direction = Random((int)directionsPatern.size() - 1);
	GetComponent<MovementComponent>()->SetDirection(directionsPatern[_direction]);
}