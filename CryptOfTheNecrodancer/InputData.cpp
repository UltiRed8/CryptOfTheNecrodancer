#include "InputData.h"
#include "InputManager.h"

void InputData::Register()
{

}

void InputData::DoAction() const
{
	if (callback) callback();
}
