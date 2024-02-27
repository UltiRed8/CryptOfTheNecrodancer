#include "MusicManager.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "Map.h"
#include "Player.h"
#include "MovementComponent.h"
#include "RythmComponent.h"

MusicManager::MusicManager()
{
	volume = 1.f;
	rythmLoop = nullptr;
	isRunning = false;
	acceptDelay = 300;
}

void MusicManager::Play(const string& _path, const int _bpm)
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
	
	new Timer("StartMusicDelay", [&]() { GetCurrent()->play(); }, milliseconds(acceptDelay/2), 1, true);
	_music->setVolume(volume);
	UpdateLoop(_bpm);
	isRunning = true;
}

void MusicManager::Toggle()
{
	isRunning = !isRunning;
	isRunning ? Unpause() : Pause();
}

void MusicManager::Pause()
{
	GetCurrent()->pause();
	if (rythmLoop)
	{
		rythmLoop->Pause();
	}
}

void MusicManager::Unpause()
{
	GetCurrent()->play();
	if (rythmLoop)
	{
		rythmLoop->Resume();
	}
}

void MusicManager::UpdateLoop(const int _bpm)
{
	if (rythmLoop)
	{
		rythmLoop->Destroy();
	}

	rythmLoop = new Timer("Timer", [this]() {

		for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
		{
			if (RythmComponent* _component = _entity->GetComponent<RythmComponent>())
			{
				_component->BeforeUpdate();
			}
		};

		new Timer("InputsTooSoon", [this]() {
			Map::GetInstance().Update();
			
			for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
			{
				if (RythmComponent* _component = _entity->GetComponent<RythmComponent>())
				{
					_component->TimedUpdate();
				}
			};
		}, milliseconds(acceptDelay / 2), 1, true);




		new Timer("InputsTooLate", [this]() {
			for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
			{
				if (RythmComponent* _component = _entity->GetComponent<RythmComponent>())
				{
					_component->AfterUpdate();
				}
			};
		}, milliseconds(acceptDelay), 1, true);

	}, seconds(1.f / (_bpm / 60.f)), -1);
}