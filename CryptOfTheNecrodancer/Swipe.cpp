#include "Swipe.h"
#include "Macro.h"
#include "MovementComponent.h"
#include "EntityManager.h"
#include "AnimationComponent.h"

Swipe::Swipe(const string& _path, const int _amount, const float _size, const bool _shouldRotate) : Entity(STRING_ID("Swipe"), "Items/Swipes/"+_path, GetSwipePosition())
{
	CreateSwipe(_amount, _size, _shouldRotate);
}

Swipe::Swipe(const int _length, const string& _path, const int _amount, const float _size, const bool _shouldRotate) : Entity(STRING_ID("Swipe"), "Items/Swipes/" + _path, GetSwipePosition())
{
	CreateSwipe(_amount, _size, _shouldRotate);
	shape->setScale((const float)_length, 1.0f);
	shape->move(GetDirection() * (_length / 2.0f) * TILE_SIZE - (GetDirection() * TILE_SIZE / 2.0f));
}

void Swipe::CreateSwipe(const int _amount, const float _size, const bool _shouldRotate)
{
	zIndex = 5;

	shape->move(Vector2f(0.0f, -0.5f) * TILE_SIZE);

	const Texture* _texture = shape->getTexture();

	size = Vector2i(_texture->getSize().x / _amount, _texture->getSize().y);

	const Vector2f& _direction = GetDirection();

	SetOriginCentered(shape);

	map<string, int> _directions = {
		{ "0-1", 270 },
		{ "01", 90 },
		{ "-10", 180 },
		{ "10", 0 },
	};

	string _value = to_string((int)_direction.x) + to_string((int)_direction.y);

	if (_shouldRotate)
	{
		shape->setRotation((const float)_directions[_value]);
	}

	shape->setScale(_size, _size);

	if (_amount > 1)
	{
		components.push_back(new AnimationComponent(this,
			{ AnimationData("Swipe", Vector2f(size), 0, _amount, 0.05f, true),
			}, "Swipe", shape));
	}

	new Timer(STRING_ID("SwipeEndTimer"), [this]() { Destroy(); }, seconds(0.05f * (_amount + 1)), 1, true);
}