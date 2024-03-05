#pragma once

#include "IManager.h"
#include "SoundData.h"
#include "Singleton.h"

struct DirectionalSettings
{
	float minDistance;
	float attenuationSpeed;
	Vector2f position;

public:
	DirectionalSettings() = default;
	DirectionalSettings(const Vector2f& _position, const float _minDistance, const float _attenuationSpeed)
	{
		position = _position;
		minDistance = _minDistance;
		attenuationSpeed = _attenuationSpeed;
	}
};

class SoundManager : public Singleton<SoundManager>, public IManager<string, SoundData>
{
	float* volume;
	float tempVolume;
	bool isMuted;

public:
	SoundManager();
	~SoundManager();

public :
	float* GetVolume() const
	{
		return volume;
	}

public:
	void Play(const string& _path, const DirectionalSettings& _settings = DirectionalSettings());
	void Stop(const string& _path);
	void IncreaseVolume();
	void DecreaseVolume();
	void ToggleVolume();
};