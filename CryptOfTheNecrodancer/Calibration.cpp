#include "Calibration.h"
#include "Macro.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "TimerManager.h"
#include "MusicManager.h"

Calibration::Calibration(const function<void()> _endCallback)
{
	timer = nullptr;
	tries = 20;
	calibrationValue = vector<float>();

	currentTime = 0;
	currentTry = 0;

	new ActionMap("Calibration", {
		{ ActionData("Click",[&]() {RegisterValue(); },{Event::KeyPressed, Keyboard::Space})}
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
	EraseElement(calibrationValue, calibrationValue[0]);
	EraseElement(calibrationValue, calibrationValue[0]);
	float _sum = 0;
	for (const int _value : calibrationValue)
	{
		_sum += _value;
	}
	const int _size = (int)(calibrationValue.size());
	latency = _sum / _size - 500;
	if (latency < -12.0f || latency > 12.0f)
	{
		latency = latency < 0.0f ? -12.0f : 12.0f;
		cout << "Latency capped!" << endl;
	}
	cout << "Latency: " << latency << endl;
}

void Calibration::Start()
{
	timer = new Timer(STRING_ID("Calibration"), [&]() {
		SoundManager::GetInstance().Play("Assets/Sounds/en_general_hit.ogg");
		currentTry++;
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
