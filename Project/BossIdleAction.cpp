#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossIdleAction.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossIdleAction::BossIdleAction()
	: maxFrameCount(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
BossIdleAction::~BossIdleAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossIdleAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isPriority			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->maxFrameCount			 = 0;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossIdleAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*アニメーションの設定*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::IDLE));

	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*使用する値の準備*/
	const float  SPEED = 0.0f;
	const VECTOR ROTATION = _boss.GetRigidbody().GetRotation();
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);			 //向き

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

	/*アニメーションの再生*/
	_boss.PlayAnimation();

	//フレーム計測
	bool isEndCount = FrameCount(this->maxFrameCount);
	//フレーム計測が終了していたら
	if (isEndCount)
	{
		OffIsSelect(0);
		_boss.SetAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossIdleAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->parameter->desireValue = 0;
	this->isPriority = false;

	///*もしHPが０以下だったら欲求値を０にして優先フラグを下す*/
	//if (_boss.GetHP() <= 0)
	//{
	//	this->isPriority = false;
	//	return;
	//}

	///*攻撃コンボが残っていなかったら欲求値を最大にする。*/
	//else if (_boss.GetAttackComboCount() == 0)
	//{
	//	this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	//	this->isPriority = true;
	//}

	///*ボスのAngryTypeをもとに最大フレームも決めておく*/
	//int angryState = _boss.GetAngryState();
	//this->maxFrameCount = json.GetJson(JsonManager::FileType::ENEMY)["IDLE_ACTION_MAX_FRAME"][angryState];
}