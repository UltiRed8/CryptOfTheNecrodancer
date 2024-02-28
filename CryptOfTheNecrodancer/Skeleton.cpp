#include "Skeleton.h"

#include"DamageComponent.h"
#include"LifeComponent.h"
#include "RythmComponent.h"
#include "AnimationComponent.h"

#include "EntityManager.h"

Skeleton::Skeleton(const Vector2f& _position, const string& _path) : Enemy(STRING_ID("Skeleton"), _path, _position)
{
    skeletonType = SK_NONE;
    cooldown = 2;
    AnimationData _animation = AnimationData(STRING_ID("Skeleton"), Vector2f(0, 0), Vector2f(24, 24), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
    components.push_back(new DamageComponent(this, 0.5));
    components.push_back(new LifeComponent(this, [this]() {DieEvent(); }, false, 1));
    components.push_back(new RythmComponent(this, nullptr, [this]() { UpdateRythm(); }, nullptr));
    components.push_back(new AnimationComponent(this, _path, { _animation }, ANIM_DIR_NONE));
}

void Skeleton::DieEvent()
{
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
        GetComponent< MovementComponent>()->SetDirection(Vector2i(_directionX,0), false);
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
    if (currentCooldown <= 0)
    {
        currentCooldown = cooldown;
        SelectDirection();
    }
}
