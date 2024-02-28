#include "MusicManager.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "Map.h"
#include "Player.h"
#include "UIImage.h"
#include "MovementComponent.h"
#include "RythmComponent.h"
#include "MenuManager.h"
#include "LightningManager.h"

MusicManager::MusicManager()
{
	volume = new float(10.f);
	rythmLoop = nullptr;
	isRunning = false;
	acceptDelay = 300;
	playSpeed = 1.0f;
	currentBPM = 0;
}

MusicManager::~MusicManager()
{
	delete volume;
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
	_music->setVolume(*volume);
	UpdateLoop(_bpm);
	isRunning = true;
}

void MusicManager::PlayMusicOnPosition(const string& _path, const int _bpm, const Vector2f& _position)
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

	new Timer("StartMusicDelay", [&]() { GetCurrent()->play(); }, milliseconds(acceptDelay / 2), 1, true);
	_music->setVolume(*volume);
	UpdateLoop(_bpm);
	isRunning = true;

	_music->setAttenuation(100.0f);
	_music->setRelativeToListener(true);
	_music->setPosition(_position.x, _position.y, 0);
	_music->setMinDistance(0.0f);
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

void MusicManager::SpeedUp()
{
	if (playSpeed != 1.0f) return;
	cout << "SpeedUp!" << endl;
	SetPlaySpeed(1.5f);
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
	}, seconds(5.0f), 1, true);
}

void MusicManager::SpeedDown()
{
	if (playSpeed != 1.0f) return;
	cout << "SpeedDown!" << endl;
	SetPlaySpeed(0.5f);
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
	}, seconds(5.0f), 1, true);
}

void MusicManager::SetPlaySpeed(const float _newValue)
{
	playSpeed = _newValue;
	GetCurrent()->setPitch(playSpeed);
	rythmLoop->SetDuration(seconds(1.f / ((currentBPM* playSpeed) / 60.f)));
}

void MusicManager::IncreaseVolume()
{
	if (*volume >= 0.f && *volume < 100.f)
	{
		GetCurrent()->setVolume(*volume += 1.f);
	}

	else if (*volume >= 100)
	{
		GetCurrent()->setVolume(100);
	}
}

void MusicManager::DecreaseVolume()
{
	if (*volume > 0.f && *volume <= 100.f)
	{
		GetCurrent()->setVolume(*volume -= 1.f);
	}
	
	else if (*volume <= 0)
	{
		GetCurrent()->setVolume(0);
	}
}

void MusicManager::ToggleVolume()
{
	if (GetCurrent()->getVolume() > 0)
	{
		GetCurrent()->setVolume(0);
		tempVolume = *volume;
		*volume = 0;
	}
	else
	{

		*volume = tempVolume;
		GetCurrent()->setVolume(*volume);
	}
}

void MusicManager::UpdateLoop(const int _bpm)
{
	if (rythmLoop)
	{
		rythmLoop->Destroy();
	}

	currentBPM = _bpm;

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

			Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("HUD")->GetElements()[0])->GetShape();
			TextureManager::GetInstance().Load(_shape, "RythmHearts1.png");
			new Timer("HeartIndicatorReset", [this]() {
				Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("HUD")->GetElements()[0])->GetShape();
				TextureManager::GetInstance().Load(_shape, "RythmHearts0.png");
			}, seconds(0.1f), 1, true);
			
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