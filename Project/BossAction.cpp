#include <DxLib.h>
#include "ActionParameter.h"
#include "BossAction.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossAction::BossAction()
	: parameter(nullptr)
{
	this->parameter = new ActionParameter();
}

/// <summary>
/// 欲求値の取得
/// </summary>
const int BossAction::GetDesireValue() const
{
	return this->parameter->desireValue;
}