#include "Inventory.h"
#include "MenuManager.h"
#include "Macro.h"

//HUD FRAME
#define I_SHOVEL "UI/hud_shovel.png"
#define I_ATTACK "UI/hud_attack.png"
#define I_BODY "UI/hud_body.png"
#define I_HEAD "UI/hud_head.png"
#define I_FEET "UI/hud_feet.png"
#define I_THROW "UI/hud_throw.png"
#define I_BOMB "UI/hud_bomb.png"
#define I_ITEM "UI/hud_item.png"

#define SHOVEL "Items/Shovel/Shovel.png"
#define PICKAXE "Items/Shovel/Pickaxe.png"

Inventory::Inventory()
{

	float _x = static_cast<float>(SCREEN_WIDTH / 2);
	unsigned int _windowY = SCREEN_HEIGHT;
	//items.push_back(new Shovel);
	new Menu("PlayerInventory", { }, 2);

}

Inventory::~Inventory()
{
	for (Item* _item : items)
	{
		_item->Destroy();
	}
}

void Inventory::AddItem(Item* _item)
{
}

void Inventory::RemoveItem(Item* _item)
{
}