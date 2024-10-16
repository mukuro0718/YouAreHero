#include <DxLib.h>
#include "UseSTL.h"
#include "Timer.h"
#include "LoadingAsset.h"

/// <summary>
/// コンストラクタ
/// </summary>
Timer::Timer()
{
	auto& asset =  Singleton<LoadingAsset>::GetInstance();
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_50_32);
}

/// <summary>
/// 描画
/// </summary>
const void Timer::Draw()const
{
	int second = this->time / 60;
	int minute = second / 60;
	second -= minute * 60;

	DrawFormatStringToHandle(1650, 10, this->TEXT_COLOR, this->fontHandle, "経過時間\n %d分%d秒", minute, second);
}