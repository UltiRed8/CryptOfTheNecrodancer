#include "Heart.h"
#include "UIImage.h"
#include "TextureManager.h"
#include "ActionMap.h"
#include "Timer.h"
#include "Macro.h"

#define PATH_HEART "FullHeart.png"
#define PATH_EMPTY_HEART "EmptyHeart.png"
#define PATH_HALFHEART "HalfHeart.png"

Heart::Heart(const string& _id, const Vector2f& _size, const Vector2f& _position, const State& _state, const float& _maxLife) : UIImage(_id, _position, _size, "")
{
	SetOriginCentered(shape);
	currentLife = _maxLife;
	maxLife = _maxLife;
	state = _state;
	path = "FullHeart.png";
	UpdateLife();
}

void Heart::UpdateLife() //Change la vie
{
	TextureManager::GetInstance().Load(shape, GetPathWithState());
}

void Heart::UIHeart() //Change taille imageAnimation
{
	if (state == H_EMPTY)
	{
		return;
	}

	UIImage::GetShape()->setScale(Vector2f(1.2f, 1.2f));
	function<void()> _heartBeat = [this]() { UIImage::GetShape()->setScale(Vector2f(1.0f, 1.0f)); };
	new Timer(STRING_ID("ResetUiHeart"), _heartBeat, seconds(0.1f), 1, true);
}

void Heart::Update(const Vector2i& _mousePosition)
{
	UpdateLife();
}
