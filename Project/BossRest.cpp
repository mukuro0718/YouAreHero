#include <DxLib.h>
#include "UseJson.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "BossRest.h"
#include "PlayerManager.h"

BossRest::BossRest()
{

}

BossRest::~BossRest()
{

}

void BossRest::Initialize()
{
}

void BossRest::Finalize()
{

}

Rigidbody& BossRest::Update(Character& _boss)
{
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRest::CalcParameter(const int _hp, const int _angryValue, const float _distance)
{
	/*追加する欲求値*/
	int addDesireValue = 0;

	/*HPが０以下だったら欲求値を0にする*/
	if (_hp <= 0)
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*怒りのゲージに比例して、欲求ゲージを決める*/
	else
	{
		this->parameter->desireValue = this->parameter->MAX_PARAMETER - _angryValue;
	}
}