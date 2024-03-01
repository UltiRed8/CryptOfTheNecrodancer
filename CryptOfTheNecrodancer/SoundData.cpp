#include "SoundData.h"
#include "SoundManager.h"

SoundData::SoundData(const string& _path) : IManagable(_path)
{
	Register();
	buffer.loadFromFile(_path);
	setBuffer(buffer);
}

void SoundData::Register()
{
	SoundManager::GetInstance().Add(id, this);
}