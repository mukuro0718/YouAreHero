#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossRoarAction.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossRoarAction::BossRoarAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
BossRoarAction::~BossRoarAction()
{

}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRoarAction::Update(Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*使用する値の準備*/
	const float  SPEED		= 0.0f;								 //スピード
	const VECTOR ROTATION	= _boss.GetRigidbody().GetRotation();//
		  VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);		 //向き

	/*スピードを０にする*/
	_boss.SetSpeed(SPEED);
	
	/*回転率をもとに、移動する向きを出す*/
	direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));

	/*向きベクトルを正規化*/
	direction = VNorm(direction);

	/*移動ベクトルを出す（重力を加算するため、Yベクトルのみ前のベクトルを使用する）*/
	VECTOR aimVelocity = VScale(direction, SPEED);								//算出された移動ベクトル
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//前の移動ベクトル
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//新しい移動ベクトル

	/*移動ベクトルの設定*/
	_boss.SetVelocity(newVelocity);

	/*アニメーション再生時間の設定*/
	_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());

	/*咆哮中にアニメーションが終了していたら、選択フラグを下してフェーズを統一する*/
	if (this->isSelect && _boss.GetIsChangeAnimation())
	{
		_boss.UnifyPhases();
		this->OffIsSelect(0);
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRoarAction::CalcParameter(const Boss& _boss)
{
	/*追加する欲求値*/
	int addDesireValue = 0;

	/*もしフェーズが異なっていたら欲求値を最大にする*/
	if (_boss.GetNowPhase() != _boss.GetPrevPhase())
	{
		addDesireValue = this->parameter->MAX_PARAMETER;
	}

	/*HPが０以下だったら欲求値を０にする*/
	else if (_boss.GetHP() <= 0)
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*欲求値を増加させる*/
	this->parameter->AddDesireValue(addDesireValue);
}