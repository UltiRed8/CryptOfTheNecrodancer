#include "Skeleton.h"

#include"DamageComponent.h"
#include"LifeComponent.h"
#include "RythmComponent.h"
#include "AnimationComponent.h"
#include "LightningComponent.h"
#include "EntityManager.h"

Skeleton::Skeleton(const float _maxHp, const float _maxDammage, const Vector2f& _position, const string& _path, const int _droppedCoins) 
	: Enemy(_maxHp,_maxDammage,STRING_ID("Skeleton"), "", _position, _droppedCoins)
{
	skeletonType = SK_NONE;
	cooldown = 2;
	//components.push_back(new AnimationComponent(this, _path, {
	/*	AnimationData(STRING_ID("SkeletonIdle"), Vector2f(0, 0), Vector2f(24, 26), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f),
		AnimationData(STRING_ID("SkeletonAttack"), Vector2f(24*4, 0), Vector2f(24, 26), READ_RIGHT, ANIM_DIR_UP, true, 4, 0.1f),
	}, ANIM_DIR_NONE));*/
}

void Skeleton::SelectDirection()
{
	// Get Player Pos
	// Set Direction to Player Pos
	Vector2i _playerPos = static_cast<Vector2i>(EntityManager::GetInstance().Get("Player")->GetPosition());
	Vector2i _skeletonPos = static_cast<Vector2i>(GetPosition());

	Vector2i _direction = Vector2i(0, 0);

	if (_playerPos.y != _skeletonPos.y)
	{
		_direction.y = _playerPos.y - _skeletonPos.y;
		_direction.y = (_direction.y > 0) ? 1 : -1;
	}
	else
	{
		_direction.x = _playerPos.x - _skeletonPos.x;
		_direction.x = (_direction.x > 0) ? 1 : -1;
	}
	GetComponent< MovementComponent>()->SetDirection(_direction);

}

void Skeleton::UpdateRythm()
{
	currentCooldown--;
	/*if (currentCooldown == 1) GetComponent<AnimationComponent>()->SetDirection(ANIM_DIR_UP);
	else GetComponent<AnimationComponent>()->SetDirection(ANIM_DIR_NONE);*/
	if (currentCooldown <= 0)
	{
		currentCooldown = cooldown;
		SelectDirection();
	}
}
