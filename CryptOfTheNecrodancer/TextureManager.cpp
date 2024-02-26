#include "TextureManager.h"

#include <iostream>

void TextureManager::Load(Shape* _shape, const string& _path, const bool _isRepeated, const bool _isSmooth)
{
	if (_path == "") return;

	TextureData* _textureData = Get(_path);

	if (!_textureData)
	{
		_textureData = new TextureData(_path);
		if (!_textureData->loadFromFile(_path))
		{
			cerr << "La texture n'a pas été correctement chargée !" << endl;
		}

		_textureData->setRepeated(_isRepeated);
		_textureData->setSmooth(_isSmooth);

		_shape->setTexture(_textureData);
		_textureData->SetRect(_shape->getTextureRect());
	}
	else
	{
		_shape->setTexture(_textureData);
	}

	_shape->setTextureRect(_textureData->GetRect());
}
