#include "TextureData.h"
#include "TextureManager.h"

TextureData::TextureData(const string& _path) : IManagable(_path)
{
	rect = IntRect();
	Register();
}

void TextureData::Register()
{
	TextureManager::GetInstance()->Add(id, this);
}