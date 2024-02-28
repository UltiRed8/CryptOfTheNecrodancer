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
	FloatRect area;

public :
	FloatRect GetDefaultZoom()
	{
		return area;
	}
	// Listener* listener;

public:
	Camera(const string& _id, const CameraType& _type, const Vector2f& _from, const Vector2f& _to);
	
public:
	void Update();

private:
	virtual void Register() override;
	void Init();
	void InitPosition();
};