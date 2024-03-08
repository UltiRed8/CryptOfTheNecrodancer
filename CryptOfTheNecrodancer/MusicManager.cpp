
#include "MusicManager.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "Map.h"
#include "Player.h"
#include "UIImage.h"
#include "MovementComponent.h"
#include "MenuManager.h"
#include "LightningManager.h"
#include "Heart.h"
#include "RythmIndicator.h"
#include "SoundManager.h"

#define PATH_RYTHM_INDICATOR_BLUE "UI/BeatMarkerBlue.png"
#define PATH_RYTHM_INDICATOR_RED "UI/BeatMarkerRed.png"
#define PATH_HEART1 "UI/RythmHearts0.png"
#define PATH_HEART2 "UI/RythmHearts1.png"
#define PATH_STAIR "Dungeons/Stairs.png"

#define SOUND_RYTHM_FAILED "Assets/Sounds/sfx_missedbeat.ogg"

MusicManager::MusicManager()
{
	musicPackName = new int(1);
	isFreeMove = false;
	latency = 0.0f;
	volume = new float(10.f);
	isRunning = false;
	acceptDelay = new float(200);
	minAcceptDelay = 0.0f;
	maxAcceptDelay = 450.0f;
	playSpeed = 1.0f;
	tempVolume = 0.0f;
	delta = 0.0f;
	beatIndex = 0;
	currentBeat = 0;
	lastBeat = 0;
	largestWait = 0;
	isLoop = false;
	shortestWait = 0;
	first = 0;
	last = 0;
	lastMaxValue = 0;
}

MusicManager::~MusicManager()
{
	delete volume;
	delete musicPackName;
	delete acceptDelay;
}

void MusicManager::Update()
{
	GarbageCollector();

	if (!isRunning) return;

	delta += TimerManager::GetInstance().GetDeltaTime() * playSpeed;

	if (delta >= currentBeat)
	{
		lastBeat = currentBeat;
		if (beatIndex == beats.size())
		{
			if (isLoop)
			{
				beatIndex = 0;
				delta = -largestWait;
				lastMaxValue = currentBeat;
				currentBeat = beats[0];
				lastBeat = 0;
				UpdateEntitiesAnimations(largestWait * (((1.0 - playSpeed) * -1.0f) + 1.0f));
				SpawnBars(largestWait);
			}
			else
			{
				return;
			}
		}
		first = lastBeat - *acceptDelay / 2.0f;
		last = lastBeat + *acceptDelay / 2.0f;
		lastMaxValue = currentBeat;
		currentBeat = beats[beatIndex];
		if (lastBeat != 0)
		{
			const int _delay = (beats[beatIndex] - lastBeat);
			SpawnBars(_delay);
			UpdateEntitiesAnimations(_delay * (((1.0 - playSpeed) * -1.0f) + 1.0f));
			if (largestWait < _delay)
			{
				largestWait = _delay;
			}
		}
		TriggerEvent();
		beatIndex++;
	}
}

MusicData* MusicManager::GetMusic(const string& _path, const Vector2f& _position)
{
	MusicData* _music = Get(_path);

	if (!_music)
	{
		_music = new MusicData(_path);
		if (!_music->openFromFile("Assets/Music/" + _path + ".ogg"))
		{
			cerr << "La musique n'a pas ete correctement chargee ! (" << _path << ".ogg)" << endl;
			return nullptr;
		}
	}

	_music->setVolume(*volume);
	_music->setPosition(Vector3f(_position.x + TILE_SIZE.x / 2.0f, _position.y + TILE_SIZE.y / 2.0f, 0.0f));

	return _music;
}

void MusicManager::PrepareMusic(const string& _path, const bool _withShopkeeper, const bool _shouldLoop)
{
	isRunning = false;
	Prepare(_path, false, _shouldLoop);
	if (_withShopkeeper)
	{
		Prepare(_path, _withShopkeeper, _shouldLoop);
	}
}

void MusicManager::Prepare(const string& _path, const bool _isShopkeeper, const bool _shouldLoop)
{
	if (_path == "") return;
	beatIndex = 0;

	StopAll();
	
	Vector2f _position = Vector2f(0.0f, 0.0f);
	string _musicPath = to_string(*musicPackName) + "/" + _path;

	Load(_musicPath);

	if (_isShopkeeper)
	{
		_position = Map::GetInstance().GetShopkeeper()->GetPosition();
		_musicPath += "_shopkeeper";
	}

	MusicData* _music = GetMusic(_musicPath, _position);
	prepared.push_back(_music);
	_music->setLoop(_shouldLoop);
	isLoop = _shouldLoop;

	if (_isShopkeeper)
	{
		_music->setVolume(*volume == 0.0f ? 0.0f : *volume + 50.0f);
		_music->setMinDistance(75.0f);
		_music->setAttenuation(3.0f);
	}
}

void MusicManager::Play()
{
	isRunning = true;
	playSpeed = 1.0f;
	delta = 0.0f;
	beatIndex = 0;
	currentBeat = 0;
	lastBeat = 0;
	largestWait = 0;
	for (MusicData* _data : prepared)
	{
		_data->play();
	}
	prepared.clear();
}

void MusicManager::Play(const string& _path, const bool _shouldLoop)
{
	if (_path == "") return;

	string _musicPath = to_string(*musicPackName) + "/" + _path;

	Load(_musicPath);

	MusicData* _music = GetMusic(_musicPath, Vector2f(0.0f, 0.0f));
	_music->setLoop(_shouldLoop);
	_music->play();
}

void MusicManager::Load(const string& _path)
{
	ifstream _in = ifstream("Assets/Music/" + _path + ".txt");

	if (!_in)
	{
		cerr << "Impossible de charger le fichier !" << endl;
		return;
	}

	beats.clear();

	string _line;
	
	string _text;

	while(getline(_in, _line))
	{
		_text += _line;
	}

	beats = VectorToInt<int>(Split(_text, ","));

	shortestWait = beats[1] - beats[0];

	const float _distance = SCREEN_WIDTH / 2.0f;

	Menu* _hud = MenuManager::GetInstance().Get("HUD");

	for (UIElement* _element : _hud->GetAllValues())
	{
		if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
		{
			_indicator->Destroy();
		}
	}
}

void MusicManager::UpdateEntitiesAnimations(const int _delay)
{
	for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
	{
		if (AnimationComponent* _animationComponent = _entity->GetComponent<AnimationComponent>())
		{
			for (Animation* _animation : _animationComponent->GetAllValues())
			{
				_animation->SetDuration(_delay / 1000.0f);
			}
		}
	}
}

void MusicManager::Toggle()
{
	isRunning = !isRunning;
	isRunning ? Unpause() : Pause();
}

void MusicManager::Pause()
{
	isRunning = false;
	for (MusicData* _music : GetAllValues())
	{
		if (_music->getStatus() == SoundSource::Playing)
		{
			_music->pause();
		}
	}
}

void MusicManager::StopAll()
{
	isRunning = false;
	for (UIElement* _element : MenuManager::GetInstance().Get("HUD")->GetAllValues())
	{
		if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
		{
			_indicator->Destroy();
		}
	}

	for (MusicData* _music : GetAllValues())
	{
		_music->stop();
	}
}

void MusicManager::Unpause()
{
	isRunning = true;
	for (MusicData* _music : GetAllValues())
	{
		if (_music->getStatus() == SoundSource::Paused)
		{
			_music->play();
		}
	}
}

void MusicManager::SpeedUp()
{
	if (playSpeed != 1.0f) return;
	SetPlaySpeed(1.125f);
	for (UIElement* _element : MenuManager::GetInstance().Get("HUD")->GetAllValues())
	{
		if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
		{
			_indicator->SetSpeed(((1.0 - playSpeed) * -1.0f) + 1.0f);
		}
	}
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
		for (UIElement* _element : MenuManager::GetInstance().Get("HUD")->GetAllValues())
		{
			if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
			{
				_indicator->SetSpeed(((1.0 - playSpeed) * -1.0f) + 1.0f);
			}
		}
	}, seconds(6.5f), 1, true);
}

void MusicManager::SpeedDown()
{
	if (playSpeed != 1.0f) return;
	SetPlaySpeed(0.875f);
	for (UIElement* _element : MenuManager::GetInstance().Get("HUD")->GetAllValues())
	{
		if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
		{
			_indicator->SetSpeed(((1.0 - playSpeed) * -1.0f) + 1.0f);
		}
	}
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
		for (UIElement* _element : MenuManager::GetInstance().Get("HUD")->GetAllValues())
		{
			if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
			{
				_indicator->SetSpeed(((1.0 - playSpeed) * -1.0f) + 1.0f);
			}
		}
	}, seconds(6.5f), 1, true);
}

void MusicManager::SetPlaySpeed(const float _newValue)
{
	playSpeed = _newValue;

	for (MusicData* _music : GetAllValues())
	{
		if (_music->getStatus() == SoundSource::Playing)
		{
			_music->setPitch(playSpeed);
		}
	}

	UpdateEntitiesAnimations(100);
}

void MusicManager::IncreaseVolume()
{
	if (*volume <= 99.0f)
	{
		*volume += 1.0f;

		for (MusicData* _music : GetAllValues())
		{
			if (StringContains(_music->GetID(), "_shopkeeper"))
			{
				_music->setVolume(*volume == 0.0f ? 0.0f : *volume + 50.0f);
			}
			else
			{
				_music->setVolume(*volume);
			}
		}
	}
}

void MusicManager::DecreaseVolume()
{
	if (*volume >= 1.0f)
	{
		*volume -= 1.0f;

		for (MusicData* _music : GetAllValues())
		{
			if (StringContains(_music->GetID(), "_shopkeeper"))
			{
				_music->setVolume(*volume == 0.0f ? 0.0f : *volume + 50.0f);
			}
			else
			{
				_music->setVolume(*volume);
			}
		}
	}
}

void MusicManager::ToggleVolume()
{
	if (tempVolume > 0.0f)
	{
		for (MusicData* _music : GetAllValues())
		{
			_music->setVolume(0);
		}
		tempVolume = *volume;
		*volume = 0.0f;
	}
	else
	{ 
		*volume = tempVolume;
		for (MusicData* _music : GetAllValues())
		{
			if (StringContains(_music->GetID(), "_shopkeeper"))
			{
				_music->setVolume(*volume == 0.0f ? 0.0f : *volume + 50.0f);
			}
			else
			{
				_music->setVolume(*volume);
			}
		}
	}
}

void MusicManager::SpawnBars(const int _delay)
{
	string _path;
	if (isLoop)
	{
		_path = PATH_RYTHM_INDICATOR_BLUE;
	}
	else
	{
		const int _amount = beats.size();
		const float _percent = (float)beatIndex / (float)_amount;
		_path = _percent > 0.8f ? PATH_RYTHM_INDICATOR_RED : PATH_RYTHM_INDICATOR_BLUE;
	}

	Menu* _hud = MenuManager::GetInstance().Get("HUD");

	const float _distance = SCREEN_WIDTH / 2;
	const float _difference = _distance * ((float)_delay / 2.0f / (float)shortestWait);
	const float _posX1 = _distance - _difference;
	const float _posX2 = _distance + _difference;
	const float _speed = _distance / shortestWait * (((1.0 - playSpeed) * -1.0f) + 1.0f);

	new RythmIndicator(RID_RIGHT, _hud, _path, _speed / 2.0f, _posX1);
	new RythmIndicator(RID_LEFT, _hud, _path, _speed / 2.0f, _posX2);
}

void MusicManager::TriggerEvent()
{
	if (!dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->GetPressingKeys())
	{
		if (dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->ResetChainMultiplier())
		{
			SoundManager::GetInstance().Play(SOUND_RYTHM_FAILED);
		}
	}
	Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("HUD")->Get("RythmHearts"))->GetShape();
	TextureManager::GetInstance().Load(_shape, PATH_HEART2);
	new Timer("HeartIndicatorReset", [this]() {
		Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("HUD")->Get("RythmHearts"))->GetShape();
		TextureManager::GetInstance().Load(_shape, PATH_HEART1);
		}, seconds(0.1f), 1, true);
	EntityManager::GetInstance().Update();

	LightningManager::GetInstance().Update();
	dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->UpdateHeartAnimation();
	Map::GetInstance().Update();
}