#include <cassert>
#include <DxLib.h>
#include <list>
#include "Mylib.h"

/// <summary>
/// Õ“Ë•¨‚Ì“o˜^
/// </summary>
void Physics::Entry(Collidable* _collidable)
{
	/*“o˜^‚³‚ê‚Ä‚¢‚é‚©’²‚×‚é*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*“o˜^‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç*/
	if (!found)
	{
		collidables.emplace_back(_collidable);
	}
	/*“o˜^‚³‚ê‚Ä‚¢‚½‚çƒGƒ‰[‚ğ“f‚­*/
	else
	{
		assert(0 && "w’è‚Ìcollidable‚Í“o˜^Ï‚İ‚Å‚·");
	}
}

/// <summary>
/// Õ“Ë•¨‚Ì“o˜^‰ğœ
/// </summary>
void Physics::Exit(Collidable* _collidable)
{
	/*“o˜^‚³‚ê‚Ä‚¢‚é‚©’²‚×‚é*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*“o˜^‚³‚ê‚Ä‚¢‚½‚ç*/
	if (!found)
	{
		collidables.remove(_collidable);
	}
	/*“o˜^‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚çƒGƒ‰[‚ğ“f‚­*/
	else
	{
		assert(0 && "w’è‚Ìcollidable‚Í“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
	}

}

/// <summary>
/// XV(“o˜^ƒIƒuƒWƒFƒNƒg‚Ì•¨—ˆÚ“®AÕ“Ë’Ê’m)
/// </summary>
void Physics::Update()
{
	// TODO:Õ“Ëî•ñ‚ÌXV‚Æ’Ê’m
	for (auto& item : collidables)
	{
		//if(Õ“Ë‚µ‚½‚ç)
		item->OnCollide();
	}
}