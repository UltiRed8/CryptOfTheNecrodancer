#include "Camera.h"
#include "CameraManager.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "Macro.h"

Camera::Camera(const string& _id, const CameraType& _type, const Vector2f& _from, const Vector2f& _to) : IManagable(_id), View(_from, _to)
{
	canRotating = new bool(true);
	type = _type;
	Register();
	Init();
	if (_type == CAMERA_PLAYER)
	{
		zoom(0.6f);
	}
	SetCameraToPlayer();
	targetRotation = 0.0f;
	currentRotation = 0.0f;
}

void Camera::Register()
{
	CameraManager::GetInstance().Add(id, this);
}

void Camera::Init()
{
	InitPosition();
}

void Camera::InitPosition()
{
	Vector2f _cameraPosition;
	_cameraPosition = type == CAMERA_PLAYER ? Vector2f(0.0f, 0.0f) : Vector2f(0.8f, 0.8f);
	
	const Vector2f& _cameraSize = Vector2f(1.0f, 1.0f);

	setViewport(FloatRect(_cameraPosition, _cameraSize));
}

void Camera::SetCameraToPlayer()
{
	Vector2f _playerPos = EntityManager::GetInstance().Get("Player")->GetPosition();
	const Vector2f& _target = _playerPos + (TILE_SIZE / 2.0f);
	setCenter(_target);
}

void Camera::Update()
{
	float _deltaTime = TimerManager::GetInstance().GetDeltaTime();

	if (*canRotating)
	{
		if (abs(currentRotation - targetRotation) > 0.08)
		{
			float _direction = currentRotation > targetRotation ? -0.08f : 0.08f;
			_direction *= _deltaTime;
			rotate(_direction);
			currentRotation += _direction;
		}
		else if (currentRotation != targetRotation)
		{
			currentRotation = targetRotation;
			setRotation(targetRotation);
		}
	};

	if (type == CAMERA_PLAYER)
	{
		Entity* _player = EntityManager::GetInstance().Get("Player");
		const Vector2f& _currentPosition = getCenter();
		const Vector2f& _target = _player->GetPosition() + (TILE_SIZE / 2.0f);
		Listener::setPosition(Vector3f(_target.x, _target.y, 0.0f));

		Vector2f _offset = Vector2f(0.0f, 0.0f);

		if (abs(_currentPosition.x - _target.x) > 10)
		{
			_offset.x = _currentPosition.x > _target.x ? -0.3f : 0.3f;
		}
		if (abs(_currentPosition.y - _target.y) > 10)
		{
			_offset.y = _currentPosition.y > _target.y ? -0.3f : 0.3f;
		}

		_offset *= _deltaTime;

		move(_offset);
	}
}