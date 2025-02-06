#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "HitStop.h"
#include "HitStopManager.h"
#include "InputManager.h"
#include "BitFlag.h"
#include "FPSController.h"

/// <summary>
/// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
/// </summary>
HitStopManager::HitStopManager()
{
}

/// <summary>
/// ƒfƒXƒgƒ‰ƒNƒ^
/// </summary>
HitStopManager::~HitStopManager()
{
	for (auto item : this->items)
	{
		DeleteMemberInstance(item);
	}
	this->items.clear();
}

/// <summary>
/// ƒqƒbƒgƒXƒgƒbƒvƒNƒ‰ƒX‚Ì“o˜^
/// </summary>
void HitStopManager::Entry(HitStop& _item)
{
	/*“o˜^‚³‚ê‚Ä‚¢‚é‚©’²‚×‚é*/
	bool found = (std::find(this->items.begin(), this->items.end(), &_item) != this->items.end());

	/*“o˜^‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç*/
	if (!found)
	{
		this->items.emplace_back(&_item);
	}
	/*“o˜^‚³‚ê‚Ä‚¢‚½‚çƒGƒ‰[‚ğ“f‚­*/
	else
	{
		//assert(0 && "w’è‚Ìitem‚Í“o˜^Ï‚İ‚Å‚·");
	}
}

/// <summary>
/// Õ“Ë•¨‚Ì“o˜^‰ğœ
/// </summary>
void HitStopManager::Exit(HitStop& _item)
{
	/*“o˜^‚³‚ê‚Ä‚¢‚é‚©’²‚×‚é*/
	bool found = (std::find(this->items.begin(), this->items.end(), &_item) != this->items.end());

	/*“o˜^‚³‚ê‚Ä‚¢‚½‚ç*/
	if (!found)
	{
		this->items.remove(&_item);
	}
	/*“o˜^‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚çƒGƒ‰[‚ğ“f‚­*/
	else
	{
		//assert(0 && "w’è‚Ìitem‚Í“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
	}

}

/// <summary>
/// XV
/// </summary>
void HitStopManager::Update()
{
	for (auto& item : this->items)
	{
		item->Update();
	}
}