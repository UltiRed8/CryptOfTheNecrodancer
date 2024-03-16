#include "CameraManager.h"
#include "InputManager.h"
#include "Macro.h"
#include "EntityManager.h"
#include "WindowManager.h"
#include "TimerManager.h"

CameraManager::CameraManager()
{
	zoomIndex = new float(0.6f);
	minimumZoomIndex = new float(0.2f);
	maximumZoomIndex = new float(1.5f);
	isShaking = false;
}

void CameraManager::Shake(const Vector2f& _direction)
{
	if (_direction == Vector2f(0.0f, 0.0f))
	{
		const int _rotation = Random(360, 0);

		const float _dirX = (const float) cos(_rotation);
		const float _dirY = (const float) sin(_rotation);

		direction = Vector2f(_dirX, _dirY);
	}
	else
	{
		direction = _direction;
	}

	const Vector2f& _offset = direction * 10.0f;

	currentPosition = Get("PlayerCamera")->getCenter() + _offset;
	Get("PlayerCamera")->setCenter(currentPosition);
	isShaking = true;
}

void CameraManager::Update()
{
	GarbageCollector();
	for (Camera* _camera : GetAllValues())
	{
		_camera->Update();
	}
	WindowManager::GetInstance().GetWindow()->setView(*Get("PlayerCamera"));

	if (!WindowManager::GetInstance().IsInFullscreen()) return;

	if (!isShaking) return;

	const Vector2f& _basePosition = EntityManager::GetInstance().Get("Player")->GetPosition();

	if (!IsNearlyEquals(currentPosition, _basePosition, 10.0f))
	{
		if (currentPosition.x != _basePosition.x)
		{
			if ((direction.x > 0.0f && currentPosition.x < _basePosition.x) || (direction.x < 0.0f && currentPosition.x > _basePosition.x))
			{
				currentPosition.x = _basePosition.x;
			}
		}
		if (currentPosition.y != _basePosition.y)
		{
			if ((direction.y > 0.0f && currentPosition.y < _basePosition.y) || (direction.y < 0.0f && currentPosition.y > _basePosition.y))
			{
				currentPosition.y = _basePosition.y;
			}
		}

		const float _delta = TimerManager::GetInstance().GetDeltaTime();
		if (currentPosition.x != _basePosition.x) currentPosition.x += currentPosition.x - _basePosition.x > 0 ? -_delta : _delta;
		if (currentPosition.y != _basePosition.y) currentPosition.y += currentPosition.y - _basePosition.y > 0 ? -_delta : _delta;
		Get("PlayerCamera")->setCenter(currentPosition);
	}
	else
	{
		isShaking = false;
	}
}

void CameraManager::ZoomIn()
{
	Camera* _player = Get("PlayerCamera");
	if (*zoomIndex < *minimumZoomIndex) return;
	const Vector2f& _cameraPosition = _player->getCenter();
	_player->reset(FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	*zoomIndex -= 0.1f;
	_player->zoom(*zoomIndex);
	_player->setCenter(EntityManager::GetInstance().Get("Player")->GetPosition());
}

void CameraManager::ZoomOut()
{
	Camera* _player = Get("PlayerCamera");
	if (*zoomIndex > *maximumZoomIndex) return;
	const Vector2f& _cameraPosition = _player->getCenter();
	_player->reset(FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	*zoomIndex += 0.1f;
	_player->zoom(*zoomIndex);
	_player->setCenter(EntityManager::GetInstance().Get("Player")->GetPosition());
}

void CameraManager::Reset()
{
	Camera* _player = Get("PlayerCamera");
	const Vector2f& _cameraPosition = _player->getCenter();
	_player->reset(FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	*zoomIndex = 0.6f;
	_player->zoom(*zoomIndex);
	_player->setCenter(EntityManager::GetInstance().Get("Player")->GetPosition());
}
