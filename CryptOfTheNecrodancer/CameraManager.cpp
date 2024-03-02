#include "CameraManager.h"
#include "InputManager.h"
#include "Macro.h"
#include "EntityManager.h"
#include "WindowManager.h"

CameraManager::CameraManager()
{
	zoomIndex = new float(0.9f);
	minimumZoomIndex = new float(0.1f);
	maximumZoomIndex = new float(1.5f);

	// TODO temp innpus, to remove!!
	new ActionMap("Camera", {
		ActionData("Zoom", [this]() { ZoomIn(); }, {Event::KeyPressed, Keyboard::Add}),
		ActionData("Dezoom", [this]() { ZoomOut(); }, {Event::KeyPressed, Keyboard::Subtract}),
	});
}

void CameraManager::Update()
{
	GarbageCollector();
	for (Camera* _camera : GetAllValues())
	{
		_camera->Update();
	}
	WindowManager::GetInstance().GetWindow()->setView(*Get("PlayerCamera"));
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
	*zoomIndex = 0.9f;
	_player->zoom(*zoomIndex);
	_player->setCenter(EntityManager::GetInstance().Get("Player")->GetPosition());
}
