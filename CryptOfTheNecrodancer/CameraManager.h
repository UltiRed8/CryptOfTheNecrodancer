#pragma once

#include "IManager.h"
#include "Singleton.h"
#include "Camera.h"

class CameraManager : public Singleton<CameraManager>, public IManager<string, Camera>
{
	float* zoomIndex;
	float* minimumZoomIndex;
	float* maximumZoomIndex;
	Vector2f currentPosition;
	Vector2f direction;
	bool isShaking;

public :
	float* GetZoomIndex()
	{
		return zoomIndex;
	}

public :
	CameraManager();

public:
	void Shake(const Vector2f& _direction);
	void Update();
	void ZoomIn();
	void ZoomOut();
	void Reset();
};