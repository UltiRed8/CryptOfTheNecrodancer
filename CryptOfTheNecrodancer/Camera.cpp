#include "Camera.h"
#include "CameraManager.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "Macro.h"

Camera::Camera(const string& _id, const CameraType& _type, const Vector2f& _from, const Vector2f& _to) : IManagable(_id), View(_from, _to)
{
	type = _type;
	Register();
	Init();
	if (_type == CAMERA_PLAYER)
	{
		zoom(0.6f);
	}
	SetCameraToPlayer();
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

		float _deltaTime = TimerManager::GetInstance().GetDeltaTime();
		_offset *= _deltaTime;

		move(_offset);
	}
}