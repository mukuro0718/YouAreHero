#include <DxLib.h>
#include "UseJson.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "BossDeath.h"
#include "PlayerManager.h"

BossDeath::BossDeath()
{

}

BossDeath::~BossDeath()
{

}

void BossDeath::Initialize()
{
}

void BossDeath::Finalize()
{

}

Rigidbody& BossDeath::Update(Character& _boss)
{
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossDeath::CalcParameter(const int _hp, const int _angryValue, const float _distance)
{
	/*追加する欲求値*/
	int addDesireValue = 0;

	/*HPが０以下だったら欲求値を最大にする*/
	if (_hp <= 0)
	{
		addDesireValue = this->parameter->MAX_PARAMETER;
	}

	/*欲求値を増加させる*/
	this->parameter->AddDesireValue(addDesireValue);
}