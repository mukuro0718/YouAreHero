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
/// 初期化
/// </summary>
void BossRoarAction::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isPriority			 = true;
	this->frameCount			 = 0;
	this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->parameter->interval	 = 0;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRoarAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*選択されていたら欲求値を０にする*/
	this->parameter->desireValue = 0;

	/*アニメーションの設定*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::ROAR));

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
	
	/*アニメーションの再生*/
	_boss.PlayAnimation();

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
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*もしHPが０以下だったら欲求値を０にして優先フラグを下す*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}
}