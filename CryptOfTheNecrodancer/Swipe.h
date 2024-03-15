#pragma once
#include "Entity.h"
#include "Player.h"

class Swipe : public Entity
{
    Vector2i size;

public:
    Swipe(const string& _path, const int _amount, const float _size = 2.0f, const bool _shouldRotate = true);
    Swipe(const int _length, const string& _path, const int _amount, const float _size = 2.0f, const bool _shouldRotate = true);

private:
    void CreateSwipe(const int _amount, const float _size, const bool _shouldRotate);

public:
    Vector2f GetDirection() const
    {
        Player* _player = (Player*)EntityManager::GetInstance().Get("Player");
        MovementComponent* _movement = _player->GetComponent<MovementComponent>();
        Vector2i* _currentDirection = _movement->GetDirection();

        return Vector2f((const float)_currentDirection->x, (const float)_currentDirection->y);
    }

    Vector2f GetSwipePosition() const
    {
        Player* _player = (Player*)EntityManager::GetInstance().Get("Player");

        const Vector2f& _direction = GetDirection();
        const Vector2f& _currentPos = _player->GetPosition();

        const Vector2f& _targetPos = _currentPos + _direction * TILE_SIZE;

        return _targetPos + TILE_SIZE / 2.0f;
    }
};