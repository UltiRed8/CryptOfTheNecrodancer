#include "SoundManager.h"

SoundManager::SoundManager()
{
	sound = Sound();
	sound.setVolume(25.0f);
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
