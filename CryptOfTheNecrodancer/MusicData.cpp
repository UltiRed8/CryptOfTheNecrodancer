#include "MusicData.h"
#include "MusicManager.h"

MusicData::MusicData(const string& _id) : IManagable(_id)
{
	Register();
}

void MusicData::Register()
{
	MusicManager::GetInstance().Add(id, this);
}
