#pragma once

#include "IManagable.h"

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
	float targetRotation;
	float currentRotation;
	bool* canRotating;

public:
	void SetRotation(const float _targetRotation)
	{
		if (!*canRotating) return;
		targetRotation = _targetRotation;
	}
	void ToggleRotating() const
	{
		*canRotating = !*canRotating;
	}

public:
	Camera(const string& _id, const CameraType& _type, const Vector2f& _from, const Vector2f& _to);
	
public:
	void Update();
	void SetCameraToPlayer();

private:
	virtual void Register() override;
	void Init();
	void InitPosition();
};