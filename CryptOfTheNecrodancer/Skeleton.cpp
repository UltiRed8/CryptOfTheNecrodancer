#include "Skeleton.h"

#include"DamageComponent.h"
#include"LifeComponent.h"
#include "RythmComponent.h"
#include "AnimationComponent.h"
#include "LightningComponent.h"
#include "EntityManager.h"

Skeleton::Skeleton(const Vector2f& _position, const string& _path) : Enemy(STRING_ID("Skeleton"), "", _position)
{
	skeletonType = SK_NONE;
	cooldown = 2;
	components.push_back(new RythmComponent(this, nullptr, [this]() { UpdateRythm(); }, nullptr));
	//components.push_back(new AnimationComponent(this, _path, {
	/*	AnimationData(STRING_ID("SkeletonIdle"), Vector2f(0, 0), Vector2f(24, 26), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f),
		AnimationData(STRING_ID("SkeletonAttack"), Vector2f(24*4, 0), Vector2f(24, 26), READ_RIGHT, ANIM_DIR_UP, true, 4, 0.1f),
	}, ANIM_DIR_NONE));*/
	InitAllBattlesComponents(50.f, 100.f);
}

void Skeleton::SelectDirection()
{
	// Get Player Pos
	// Set Direction to Player Pos
	Vector2i _playerPos = static_cast<Vector2i>(EntityManager::GetInstance().Get("Player")->GetPosition());
	Vector2i _skeletonPos = static_cast<Vector2i>(GetPosition());

	bool _isPriority = Random(1, 0);

	if (_isPriority)
	{
		int _directionX = _playerPos.x - _skeletonPos.x;
		_directionX = (_directionX > 0) ? 1 : -1, 0;
		GetComponent< MovementComponent>()->SetDirection(Vector2i(_directionX, 0), false);
	}
	else
	{
		int _directionY = _playerPos.y - _skeletonPos.y;
		_directionY = (_directionY > 0) ? 1 : -1, 0;
		GetComponent< MovementComponent>()->SetDirection(Vector2i(0, _directionY), false);
	}

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
