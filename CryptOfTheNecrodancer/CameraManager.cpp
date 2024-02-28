#include "CameraManager.h"

CameraManager::CameraManager()
{
	zoomIndex = new float(1.0f);
	minimumZoomIndex = new float(0.9f);
	maximumZoomIndex = new float(1.1f);
}

void CameraManager::Update(RenderWindow* _window)
{
	for (Camera* _camera : GetAllValues())
	{
		_camera->Update();
	}
	_window->setView(*Get("PlayerCamera"));
}

void CameraManager::ZoomIn()
{
	if (*zoomIndex <= *minimumZoomIndex) return;
	Camera _player = *Get("PlayerCamera");
	//_player.reset(_player.GetDefaultZoom());
	*zoomIndex -= 0.05f;
	_player.zoom(*zoomIndex);
}

void CameraManager::ZoomOut()
{
	if (*zoomIndex >= *maximumZoomIndex) return;
	Camera _player = *Get("PlayerCamera");
	//_player.reset(_player.GetDefaultZoom());
	*zoomIndex += 0.05f;
	_player.zoom(*zoomIndex);
	
}
