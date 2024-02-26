#include "InputData.h"
#include "InputManager.h"

void InputData::Register()
{
	//InputManager::GetInstance()->Add(id, this);
}

void InputData::DoAction() const
{
	if (callback) callback();
}
