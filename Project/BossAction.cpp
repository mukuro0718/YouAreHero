#include <DxLib.h>
#include "UseSTL.h"
#include "ReactionType.h"
#include "ActionParameter.h"
#include "HitStop.h"
#include "Character.h"
#include "BossAttack.h"
#include "Boss.h"
#include "BossAction.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossAction::BossAction()
	: parameter		(nullptr)
	, attack		(nullptr)
	, hitStop		(nullptr)
	, isSelect		(false)
	, isInitialize	(false)
	, isPriority	(false)
	, frameCount	(0)
{
	this->parameter = new ActionParameter();
	this->hitStop = new HitStop();
}

/// <summary>
/// 描画
/// </summary>
const void BossAction::Draw()const
{
	if (this->attack != nullptr)
	{
		this->attack->Draw();
	}
}

/// <summary>
/// 欲求値の取得
/// </summary>
const int BossAction::GetDesireValue() const
{
	return this->parameter->desireValue;
}

/// <summary>
/// 重みの取得
/// </summary>
const int BossAction::GetWeight(const int _sum)const
{
	return this->parameter->GetWeight(_sum);
}

/// <summary>
/// 重みのベースの取得
/// </summary>
const int BossAction::GetBaseWeight()const
{
	return this->parameter->BASE_WEIGHT;
}
/// <summary>
/// 指定したフレームの計測
/// </summary>
bool BossAction::FrameCount(const int _maxFrame)
{
	//カウントを増加させる
	this->frameCount++;
	//もし最大を越していたらフラグを下してカウントを初期化する
	if (this->frameCount >= _maxFrame)
	{
		return true;
	}
	return false;
}

void BossAction::OffIsSelect(const int _maxInterval)
{
	/*選択されていたら*/
	if (this->isSelect)
	{
		this->isSelect = false;
		this->isPriority = false;
		this->frameCount = 0;
		this->parameter->desireValue = 0;
	}
}