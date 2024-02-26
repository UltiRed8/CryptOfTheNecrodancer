#include "TextureManager.h"
#include"Macro.h"
#include <iostream>

void TextureManager::Load(Shape* _shape, const string& _path, const bool _isRepeated, const bool _isSmooth)
{
	if (_path == "") return;

	TextureData* _textureData = Get(_path);

	if (!_textureData)
	{
		_textureData = new TextureData(_path);
		if (!_textureData->loadFromFile("Assets/Textures/" + _path))
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

void TextureManager::LoadSprite(Sprite* _sprite, const string& _path, const bool _isRepeated, const bool _smooth)
{
	if (_path == "") return;

	TextureData* _textureData = Get(_path);

	if (!_textureData)
	{
		_textureData = new TextureData(_path);
		if (!_textureData->loadFromFile("Assets/Textures/" + _path))
		{
			cerr << "La texture n'a pas été correctement chargée !" << endl;
		}

		_textureData->setRepeated(_isRepeated);
		_textureData->setSmooth(_smooth);

		_sprite->setTexture(*_textureData);
		_textureData->SetRect(_sprite->getTextureRect());
	}

	else
	{
		_sprite->setTexture(*_textureData);
	}

	_sprite->setTextureRect(_textureData->GetRect());
	SetOriginCentered(_sprite);
}
