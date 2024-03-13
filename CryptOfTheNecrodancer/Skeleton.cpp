#include "Skeleton.h"

#include"DamageComponent.h"
#include"LifeComponent.h"
#include "AnimationComponent.h"
#include "LightSource.h"
#include "EntityManager.h"

#define SOUND_ATTACK_SKELETON "Assets/Sounds/en_skel_attack_melee.ogg"
#define SOUND_HIT_SKELETON "Assets/Sounds/en_skel_hit.ogg"
#define SOUND_DEATH_SKELETON "Assets/Sounds/en_skel_death.ogg"

Skeleton::Skeleton(const float _maxHp, const float _maxDammage, const Vector2f& _position, const string& _path, const int _droppedCoins) 
	: Enemy(_maxHp,_maxDammage,STRING_ID("Skeleton"), PATH_SKELETON, _position, _droppedCoins)
{
	skeletonType = SK_NONE;
	cooldown = 2;
	components.push_back(new AnimationComponent(this, {
		AnimationData("Idle", Vector2f(24, 26), 0, 3, 0.1f, true),
		AnimationData("Attack", Vector2f(24, 26), 4, 3, 0.1f, true),
	}, "Attack", visuals));

	currentCooldown = Random(2,0);

	attackSound = SOUND_ATTACK_SKELETON;
	hitSound = SOUND_HIT_SKELETON;
	deathSound = SOUND_DEATH_SKELETON;
}

void Skeleton::SelectDirection()
{
	const Vector2i& _playerPos = static_cast<Vector2i>(EntityManager::GetInstance().Get("Player")->GetPosition());
	const Vector2i& _skeletonPos = static_cast<Vector2i>(GetPosition());

	Vector2i _direction = Vector2i(0, 0);

	int _randomDirection = Random(1, 0);

	if (_playerPos.y != _skeletonPos.y && _randomDirection)
	{
		_direction.y = _playerPos.y - _skeletonPos.y;
		_direction.y = (_direction.y > 0) ? 1 : -1;
	}
	else if (_playerPos.x != _skeletonPos.x)
	{
		_direction.x = _playerPos.x - _skeletonPos.x;
		_direction.x = (_direction.x > 0) ? 1 : -1;
	}
	GetComponent<MovementComponent>()->SetDirection(_direction);
}

void Skeleton::Update()
{
	currentCooldown--;
	if (currentCooldown == 1) GetComponent<AnimationComponent>()->SetPlayingID("Attack");
	else GetComponent<AnimationComponent>()->SetPlayingID("Idle");

	if (currentCooldown <= 0)
	{
		currentCooldown = cooldown;
		SelectDirection();
	}
	for (Component* _component : components)
	{
		_component->Update();
	}
	visuals->setPosition(shape->getPosition() + Vector2f(0.0f, -0.5f) * TILE_SIZE);
}