#pragma once

#include "MusicData.h"
#include "IManager.h"
#include "Singleton.h"
#include "Timer.h"
#include <fstream>
#include <SFML/Audio.hpp>

using namespace std;

class MusicManager : public IManager<string, MusicData>, public Singleton<MusicManager>
{
	int* musicPackName;
	bool isFreeMove;
	float latency;
	float* volume;
	bool isRunning;
	float* acceptDelay;
	float minAcceptDelay;
	float maxAcceptDelay;
	float playSpeed;
	float tempVolume;
	float delta;
	vector<MusicData*> prepared;
	vector<int> beats;
	int beatIndex;
	int currentBeat;
	int lastBeat;
	bool isLoop;
	int largestWait;
	int shortestWait;

	int first;
	int last;
	int lastMaxValue;

public:
	bool IsRunning() const
	{
		return isRunning;
	}
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

	void SetLatency(const float _latency)
	{
		latency = _latency;
	}

public:
	MusicManager();
	~MusicManager();

public:
	void PrepareMusic(const string& _path, const bool _withShopkeeper = false, const bool _shouldLoop = false);
	void Prepare(const string& _path, const bool _isShopkeeper, const bool _shouldLoop);
	void Play();
	void Play(const string& _path, const bool _shouldLoop);
	void Update();
	void StopAll();
	void Toggle();
	void Pause();
	void Unpause();
	void SpeedUp();
	void SpeedDown();
	void SetPlaySpeed(const float _newValue);
	void IncreaseVolume();
	void DecreaseVolume();
	void ToggleVolume();
	void TriggerEvent();

private:
	void SpawnBars(const int _delay);
	MusicData* GetMusic(const string& _path, const Vector2f& _position);
	void UpdateEntitiesAnimations(const int _delay);
	void Load(const string& _path);
};