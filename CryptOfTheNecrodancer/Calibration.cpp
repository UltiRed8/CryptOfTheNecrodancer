#include "Calibration.h"
#include "Macro.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "TimerManager.h"
#include "MusicManager.h"
#include "MenuManager.h"

#define PATH_HEART1 "UI/RythmHearts0.png"
#define PATH_HEART2 "UI/RythmHearts1.png"

Calibration::Calibration(const function<void()> _endCallback)
{
	timer = nullptr;
	tries = 20;
	calibrationValue = vector<float>();

	currentTime = 0;
	currentTry = 0;

	new ActionMap("Calibration", {
		{ ActionData("Click",[&]() {RegisterValue(); },{Event::KeyPressed,' '})}
		});
	latency = 0;
	isFinished = false;
	endCallback = _endCallback;
}

void Calibration::RegisterValue()
{
	if (isFinished)
	{
		return;
	}
	calibrationValue.push_back(currentTime);
	cout << "Value: " << currentTime << endl;
	currentTime = 0;
}

void Calibration::ComputeMSLatency()
{
	float _sum = 0;
	const int _size = (int)(calibrationValue.size());
	for (int _index = 0; _index < _size; _index++)
	{
		_sum += calibrationValue[_index];
	}
	const int _totalMs = 500 * _size;
	const float _result = (float)(_totalMs) - _sum;
	latency = _result / _size;
	cout << "Latency: " << latency << endl;
}

void Calibration::Start()
{
	timer = new Timer(STRING_ID("Calibration"), [&]() {
		SoundManager::GetInstance().Play("Assets/Sounds/en_general_hit.ogg");
		currentTry++;
		Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("Calibration")->Get("RythmHearts"))->GetShape();
		TextureManager::GetInstance().Load(_shape, PATH_HEART2);
		new Timer("CalibHeart", [this]() {
			Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("Calibration")->Get("RythmHearts"))->GetShape();
			TextureManager::GetInstance().Load(_shape, PATH_HEART1);
		}, seconds(0.1f), 1, true);
		}, milliseconds(500),tries, false);
}

void Calibration::Update()
{
	if (isFinished)
	{
		return;
	}
	currentTime += (int)TimerManager::GetInstance().GetDeltaTime();
	if (currentTry == tries)
	{
		timer->Pause();
		timer->Destroy();
		ComputeMSLatency();
		MusicManager::GetInstance().SetLatency(latency);
		isFinished = true;
		if (endCallback)
		{
			endCallback();
		}
	}
}
