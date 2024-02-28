#include "Shopkeeper.h"

#include"MovementComponent.h"
#include"DamageComponent.h"
#include"LifeComponent.h"
#include "RythmComponent.h"
#include "AnimationComponent.h"

Shopkeeper::Shopkeeper(const Vector2f& _position) : Enemy(STRING_ID("ShopKeeper"), PATH_SHOPKEEPER, _position)
{
	directionsPatern = {
		   Vector2i(0,0),
	};

	AnimationData _animation = AnimationData("ShopKeeper", Vector2f(0, 0), Vector2f(47, 38), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
	components.push_back(new RythmComponent(this, nullptr, [this]() { UpdateRythm(); }, nullptr));
	components.push_back(new AnimationComponent(this, PATH_SHOPKEEPER, { _animation }, ANIM_DIR_NONE));
	components.push_back(new MovementComponent(this));
	components.push_back(new DamageComponent(this, 0.5));
	components.push_back(new LifeComponent(this, [this]() {DieEvent(); }, false, 1));

	cooldown = 2;

    // MUSIC

    // Time _currentMusicTime = MusicManager::GetInstance().GetCurrent()->getPlayingOffset();
	// MusicManager::GetInstance().PlayMusicOnPosition("TheWighttoRemain.mp3", 160, _position);
    
}

void Shopkeeper::DieEvent()
{
}

void Shopkeeper::SelectDirection()
{
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

void Shopkeeper::UpdateRythm()
{
    /*MusicManager::GetInstance().GetCurrent()->setPosition(GetPosition().x, GetPosition().y, 0);*/
    currentCooldown--;
    if (currentCooldown <= 0)
    {
        currentCooldown = cooldown;
        SelectDirection();
    }
}

