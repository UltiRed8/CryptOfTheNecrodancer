#include "CameraManager.h"

void CameraManager::Update(RenderWindow* _window)
{
	for (Camera* _camera : GetAllValues())
	{
		_camera->Update();
	}
	_window->setView(*Get("PlayerCamera"));
}
