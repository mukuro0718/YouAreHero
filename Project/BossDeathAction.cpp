#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossDeathAction.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossDeathAction::BossDeathAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
BossDeathAction::~BossDeathAction()
{

}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossDeathAction::Update(Boss& _boss)
{
	/*アクションが選択されているとき*/
	if (this->isSelect)
	{
		//もしアニメーションが終了していたら
		if (_boss.GetIsChangeAnimation())
		{
			_boss.OffIsAlive();
		}
	}
}
/// <summary>
/// パラメーターの計算
/// </summary>
void BossDeathAction::CalcParameter(const Boss& _boss)
{
	/*追加する欲求値*/
	int addDesireValue = 0;

	/*HPが０以下だったら欲求値を最大にする*/
	if (_boss.GetHP() <= 0)
	{
		addDesireValue = this->parameter->MAX_PARAMETER;
	}

	/*欲求値を増加させる*/
	this->parameter->AddDesireValue(addDesireValue);
}