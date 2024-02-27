#include "Camera.h"
#include "CameraManager.h"
#include "EntityManager.h"
#include "Macro.h"

Camera::Camera(const string& _id, const CameraType& _type, const Vector2f& _from, const Vector2f& _to) : IManagable(_id), View(_from, _to)
{
	type = _type;
	Register();
	Init();
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

void Camera::Update()
{
	if (type == CAMERA_PLAYER)
	{
		const Vector2f& _currentPosition = getCenter();
		const Vector2f& _playerPosition = EntityManager::GetInstance().Get("Player")->GetPosition();

		Vector2f _offset = Vector2f(0.0f, 0.0f);
		_offset.x = _currentPosition.x > _playerPosition.x ? -0.1f : 0.1f;
		_offset.y = _currentPosition.y > _playerPosition.y ? -0.1f : 0.1f;

		//move(_offset);
		setCenter(_playerPosition);
	}
}