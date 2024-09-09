#include <DxLib.h>
#include "UseSTL.h"
#include "SceneUI.h"


const void SceneUI::Image::Draw()const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->alpha);
	DrawExtendGraph(lx, this->ly, this->rx, this->ry, this->imageHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->MAX_ALPHA);
}
void SceneUI::Image::SetPosition(std::vector<int> _position)
{
	this->lx = _position[0];
	this->ly = _position[1];
	this->rx = _position[2];
	this->ry = _position[3];
}
void SceneUI::Image::AddAlpha(const int _addAlpha)
{
	/*アルファ増加フラグが立っていなければ早期リターン*/
	if (!this->isAddAlpha)return;

	this->alpha += _addAlpha;

	if (this->alpha >= this->MAX_ALPHA)
	{
		this->alpha = this->MAX_ALPHA;
		this->isAddAlpha = false;
	}
}
void SceneUI::Image::DecreaseAlpha(const int _decreaseAlpha)
{
	/*アルファ増加フラグが立っていたら早期リターン*/
	if (this->isAddAlpha)return;

	this->alpha -= _decreaseAlpha;

	if (this->alpha < 0)
	{
		this->alpha = 0;
		this->isAddAlpha = true;
	}
}

void SceneUI::Image::FlashingAlpha(const int _addAlpha)
{
	if (this->isAddAlpha)
	{
		this->AddAlpha(_addAlpha);
	}
	else
	{
		this->DecreaseAlpha(_addAlpha);
	}
}