#pragma once

#include "MusicData.h"
#include "IManager.h"
#include "Singleton.h"
#include "Timer.h"

#include <SFML/Audio.hpp>

class MusicManager : public IManager<string, MusicData>, public Singleton<MusicManager>
{
	MusicData* currentMain;
	MusicData* currentShopkeeper;
	bool debugMode;



	float* acceptDelay;
	float minAcceptDelay;
	float maxAcceptDelay;
	int currentBPM;
	bool isRunning;
	float playSpeed;
	float tempVolume;
	float* volume;
	Timer* rythmLoop;
	float delta;
	int beatDelay;
	bool didEvent;

public:
	float* GetAcceptDelay() const
	{
		return acceptDelay;
	}

	void SetAcceptDelay(const float _acceptDelay)
	{
		*acceptDelay += _acceptDelay;

		if (*acceptDelay < minAcceptDelay)
		{
			*acceptDelay = minAcceptDelay;
		}
		if (*acceptDelay > maxAcceptDelay)
		{
			*acceptDelay = maxAcceptDelay;
		}
	}

	float* GetVolume() const
	{
		return volume;
	}
	void SetVolume(const float _volume)
	{
		*volume = _volume;
		for (MusicData* _music : GetAllValues())
		{
			_music->setVolume(*volume);
		}
	}

public:
	MusicManager();
	~MusicManager();

public:
	MusicData* GetMusic(const string& _path, const Vector2f& _position);
	void PlayMain(const string& _path, const int _bpm, const bool _withShopkeeper = false, const bool _shouldLoop = false);
	void StopAll();
	void Update();
	void Toggle();
	void Pause();
	void Unpause();
	void SpeedUp();
	void SpeedDown();
	void SetPlaySpeed(const float _newValue);
	void IncreaseVolume();
	void DecreaseVolume();
	void ToggleVolume();
	bool TriggerEvent();

private:
	void UpdateLoop();
};