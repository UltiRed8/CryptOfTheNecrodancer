#pragma once

#include "IManagable.h"

#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

enum CameraType
{
	CAMERA_PLAYER,
	CAMERA_MINIMAP,
};

class Camera : public IManagable<string>, public View
{
	CameraType type;

public:
	Camera(const string& _id, const CameraType& _type, const Vector2f& _from, const Vector2f& _to);

private:
	virtual void Register() override;
	void Init();
	void InitPosition();
};