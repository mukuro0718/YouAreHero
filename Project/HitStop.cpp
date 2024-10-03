#include <DxLib.h>
#include "UseSTL.h"
#include "HitStop.h"
#include "HitStopManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
HitStop::HitStop()
	: type		(static_cast<int>(Type::NONE))
	, time		(0)
	, slowFactor(this->SLOW_MOTION_FACTOR)
{
	auto& manager = Singleton<HitStopManager>::GetInstance();
	manager.Entry(*this);
}

/// <summary>
/// デストラクタ
/// </summary>
HitStop::~HitStop()
{
	auto& manager = Singleton<HitStopManager>::GetInstance();
	manager.Exit(*this);
}
/// <summary>
/// 初期化
/// </summary>
void HitStop::Initialize()
{
	this->type		 = static_cast<int>(Type::NONE);
	this->time		 = 0;
	this->slowFactor = this->SLOW_MOTION_FACTOR;
}
/// <summary>
/// 更新
/// </summary>
void HitStop::Update()
{
	/*typeがNONEだったら早期リターン*/
	if (this->type == static_cast<int>(Type::NONE))return;

	/*待ち時間が残っていたら早期リターン*/
	if (this->delay > 0)
	{
		this->delay--;
		return;
	}

	if (this->type == static_cast<int>(Type::STOP))
	{
		this->time--;
		if (this->time < 0)
		{
			this->type = static_cast<int>(Type::NONE);
		}
	}
	else if (this->type == static_cast<int>(Type::SLOW))
	{
		this->time--;
		if (this->time < 0)
		{
			this->type = static_cast<int>(Type::NONE);
		}
	}

}


/// <summary>
/// ヒットストップ中か
/// </summary>
bool HitStop::IsHitStop()
{
	/*待ち時間が残っていたら早期リターン*/
	if (this->delay != 0) return false;

	if (this->type == static_cast<int>(Type::STOP))
	{
		return true;
	}
	else if (this->type == static_cast<int>(Type::SLOW))
	{
		if (this->time % (int)(1.0f / this->slowFactor) == 0)
		{
			return false;
		}
		return true;
	}
	return false;
}

void HitStop::SetHitStop(const float _time, const int _type, const int _delay, const float _slowFactor)
{
	this->type		 = _type;
	this->time		 = _time;
	this->slowFactor = _slowFactor;
	this->delay		 = _delay;
}