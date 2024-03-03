#pragma once

#include "MusicData.h"
#include "IManager.h"
#include "Singleton.h"
#include "Timer.h"

#include <SFML/Audio.hpp>

enum RythmType
{
	RT_NONE, RT_FREEMOVE, RT_ALL
};

class MusicManager : public IManager<string, MusicData>, public Singleton<MusicManager>
{
	MusicData* currentMain;
	MusicData* currentShopkeeper;
	RythmType rythmType;

	int* musicPackName;
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
	bool needsAnimationUpdate;

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
	int GetCurrentBPM() const
	{
		return currentBPM;
	}
	void SetCurrentBPM(const int _bpm)
	{
		currentBPM = _bpm;
	}

	int* GetMusicPackName() const
	{
		return musicPackName;
	}

	void IncreaseMusicPackName() const
	{
		*musicPackName += 1;

		if (*musicPackName >= 7)
		{
			*musicPackName = 7;
		}
	}
	void DecreaseMusicPackName() const
	{
		*musicPackName -= 1;

		if (*musicPackName <= 1)
		{
			*musicPackName = 1;
		}
	}

public:
	MusicManager();
	~MusicManager();

public:
	MusicData* GetMusic(const string& _path, const Vector2f& _position);
	void PrepareMain(const string& _path, const int _bpm, const bool _withShopkeeper = false, const bool _shouldLoop = false);
	void StopAll();
	void UpdateEntitiesAnimations();
	void Update();
	void Toggle();
	void Pause();
	void Unpause();
	void Play();
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