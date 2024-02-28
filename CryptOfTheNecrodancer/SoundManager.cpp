#include "SoundManager.h"

SoundManager::SoundManager()
{
	sound = Sound();
	volume = new float(20.f);
	sound.setVolume(*volume);
}

SoundManager::~SoundManager()
{
	delete volume;
}

void SoundManager::Play(const string& _path)
{
	if (_path == "") return;

	SoundData* _soundData = Get(_path);

	if (!_soundData)
	{
		Load(_path);
		_soundData = Get(_path);
	}

	if (_soundData)
	{
		sound.setBuffer(*_soundData);
		sound.play();
	}
}

void SoundManager::IncreaseVolume()
{
	if (*volume >= 0.f && *volume < 100.f)
	{
		sound.setVolume(*volume += 1.f);
	}

	else if (*volume >= 100)
	{
		sound.setVolume(100);
	}
}

void SoundManager::DecreaseVolume()
{
	if (*volume > 0.f && *volume <= 100.f)
	{
		sound.setVolume(*volume -= 1.f);
	}

	else if (*volume <= 0)
	{
		sound.setVolume(0);
	}
}

void SoundManager::ToggleVolume()
{
	if (sound.getVolume() > 0)
	{
		sound.setVolume(0);
		tempVolume = *volume;
		*volume = 0;
	}
	else
	{
		*volume = tempVolume;
		sound.setVolume(*volume);
	}
}

void SoundManager::Load(const string& _path)
{
	if (_path == "") return;

	SoundData* _soundData = Get(_path);

	if (!_soundData)
	{
		_soundData = new SoundData(_path);
		if (!_soundData->loadFromFile(_path))
		{
			cerr << "Le son n'a pas été correctement chargée !" << endl;
		}
	}
}
