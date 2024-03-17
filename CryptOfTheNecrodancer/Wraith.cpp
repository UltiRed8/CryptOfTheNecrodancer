#include "Wraith.h"

#include"DamageComponent.h"
#include"LifeComponent.h"
#include "AnimationComponent.h"
#include "LightSource.h"
#include "EntityManager.h"

#define SOUND_ATTACK_WRAITH "Assets/Sounds/en_wraith_attack.ogg"
#define SOUND_HIT_WRAITH "Assets/Sounds/en_wraith_cry.ogg"
#define SOUND_DEATH_WRAITH "Assets/Sounds/en_wraith_death.ogg"

Wraith::Wraith(const Vector2f& _position) : Enemy(100.0f, 50.f, STRING_ID("Wraith"), PATH_WRAITH, _position, 2)
{
	cooldown = 1;
	components.push_back(new AnimationComponent(this, {
		AnimationData("Idle", Vector2f(24, 24), 0, 2, 0.1f, true),
		}, "Idle", visuals));

	currentCooldown = Random(2, 0);

	attackSound = SOUND_ATTACK_WRAITH;
	hitSound = SOUND_HIT_WRAITH;
	deathSound = SOUND_DEATH_WRAITH;
}

void Wraith::SelectDirection()
{
	const Vector2i& _playerPos = static_cast<Vector2i>(EntityManager::GetInstance().Get("Player")->GetPosition());
	const Vector2i& _wraithPos = static_cast<Vector2i>(GetPosition());

	Vector2i _direction = Vector2i(0, 0);

	int _randomDirection = Random(1, 0);

	if (_playerPos.y != _wraithPos.y && _randomDirection)
	{
		_direction.y = _playerPos.y - _wraithPos.y;
		_direction.y = (_direction.y > 0) ? 1 : -1;
	}
	else if (_playerPos.x != _wraithPos.x)
	{
		_direction.x = _playerPos.x - _wraithPos.x;
		_direction.x = (_direction.x > 0) ? 1 : -1;
	}
	GetComponent<MovementComponent>()->SetDirection(_direction);
}

