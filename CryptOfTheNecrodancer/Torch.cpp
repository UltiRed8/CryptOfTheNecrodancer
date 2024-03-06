#include "Torch.h"
#include "Macro.h"

#define PATH_TORCH "Dungeons/wall_torch.png"

Torch::Torch(const Vector2f& _position) : Placeable(STRING_ID("Torch"), PATH_TORCH, _position)
{
	zIndex = 3;

	components.push_back(new AnimationComponent(this, {
		AnimationData("Torch", Vector2f(26, 26), 0, 3, 0.1f, false)
	}, "Torch", shape));

	shape->move(Vector2f(0.0f, -1.0f) * TILE_SIZE);

	lightSource = new LightSource(STRING_ID("TorchLightSource"), this, 100);
}

Torch::~Torch()
{
	lightSource->Destroy();
}