#pragma once

#include "IManager.h"
#include "Singleton.h"
#include "Camera.h"

class CameraManager : public Singleton<CameraManager>, public IManager<string, Camera>
{

};