#pragma once

#include "IManager.h"
#include "Singleton.h"
#include "Camera.h"

class CameraManager : public Singleton<CameraManager>, public IManager<string, Camera>
{
	float* zoomIndex;
	float* minimumZoomIndex;
	float* maximumZoomIndex;

public :
	float* GetZoomIndex()
	{
		return zoomIndex;
	}

public :
	CameraManager();

public:
	void Update(RenderWindow* _window);
	void ZoomIn();
	void ZoomOut();
	void Reset();
};