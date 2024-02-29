#include "Shopkeeper.h"
#include "LightningComponent.h"
#include"MovementComponent.h"
#include"DamageComponent.h"
#include"LifeComponent.h"
#include "RythmComponent.h"
#include "AnimationComponent.h"

Shopkeeper::Shopkeeper(const Vector2f& _position) : Enemy(1000.f,900.f,STRING_ID("ShopKeeper"), PATH_SHOPKEEPER, _position, 0)
{
	directionsPatern = {
		   Vector2i(0,0),
	};

	//AnimationData _animation = AnimationData("ShopKeeper", Vector2f(0, 0), Vector2f(47, 38), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
	components.push_back(new RythmComponent(this, nullptr, [this]() { UpdateRythm(); }, nullptr));
	//components.push_back(new AnimationComponent(this, PATH_SHOPKEEPER, { _animation }, ANIM_DIR_NONE));
    GetComponent<MovementComponent>()->SetCanMove(false);
	cooldown = 0;
}

void Shopkeeper::DieEvent()
{
}

void Shopkeeper::SelectDirection()
{
    const Vector2f& _playerPos = EntityManager::GetInstance().Get("Player")->GetPosition();
    const Vector2f& _ownPosition = GetPosition();

    Vector2i _direction = Vector2i(0, 0);

    if (_playerPos.y != _ownPosition.y)
	{
        _direction.x = (int)(_playerPos.x - _ownPosition.x);
        _direction.x = (_direction.x > 0) ? 1 : -1, 0;
    }
    else
    {
        _direction.y = (int)(_playerPos.y - _ownPosition.y);
        _direction.y = (_direction.y > 0) ? 1 : -1, 0;
    }
    GetComponent< MovementComponent>()->SetDirection(_direction, false);
}

void Shopkeeper::UpdateRythm()
{
    currentCooldown--;
    if (currentCooldown <= 0)
    {
        currentCooldown = cooldown;
        SelectDirection();
    }
}

