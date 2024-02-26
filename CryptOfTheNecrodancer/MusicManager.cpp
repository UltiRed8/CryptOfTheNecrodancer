#include "MusicManager.h"

MusicManager::MusicManager()
{
	volume = 10.f;
}

void MusicManager::Play(const string& _path)
{
	if (_path == "") return;

	MusicData* _music = Get(_path);

	if (!_music)
	{
		_music = new MusicData(_path);
		if (!_music->openFromFile("Assets/Music/" + _path))
		{
			cerr << "La texture n'a pas été correctement chargée !" << endl;
		}
	}
	
	_music->play();
	_music->setVolume(volume);
}

