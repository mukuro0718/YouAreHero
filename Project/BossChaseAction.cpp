#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossChaseAction.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossChaseAction::BossChaseAction()
{
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	this->moveDistance		= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"];
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["CHASE_INTERVAL"];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::WALK);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// デストラクタ
/// </summary>
BossChaseAction::~BossChaseAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossChaseAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossChaseAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	if (this->parameter->desireValue != 0)
	{
		this->parameter->desireValue = 0;							//欲求値を０にする
		_boss.SetSpeed(this->speed);							//速さの設定
		_boss.SetAnimationPlayTime(this->animationPlayTime);//アニメーション再生時間の設定
		_boss.SetNowAnimation(this->nextAnimation);			//アニメーションの設定
	}

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*使用する値の準備*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//座標
	this->moveTarget	  = player.GetRigidbody().GetPosition();//移動目標
	VECTOR nowRotation	  = _boss.GetRigidbody().GetRotation();	//回転率
	VECTOR direction	  = VGet(0.0f, 0.0f, 0.0f);		//向き

	/*移動ベクトルの設定*/
	_boss.SetNowMoveTarget(this->moveTarget);

	/*プレイヤーから自分の座標までのベクトルを出す*/
	VECTOR positionToTargetVector = VSub(POSITION, this->moveTarget);

	/*アークタンジェントを使って角度を求める*/
	nowRotation = GetLerpRotation(_boss, positionToTargetVector, nowRotation, this->rotateLerpValue);

	/*回転率を代入*/
	_boss.SetRotation(nowRotation);


	/*回転率をもとに、移動する向きを出す*/
	direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));

	/*向きベクトルを正規化*/
	direction = VNorm(direction);

	/*移動ベクトルを出す（重力を加算するため、Yベクトルのみ前のベクトルを使用する）*/
	VECTOR aimVelocity = VScale(direction, this->speed);					//算出された移動ベクトル
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//前の移動ベクトル
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//新しい移動ベクトル
	_boss.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	_boss.PlayAnimation();

	/*移動目標との距離が定数以下だったら選択フラグを下す*/
	float distance = VSize(positionToTargetVector);
	if (distance <= this->moveDistance)
	{
		OffIsSelect(this->maxInterval);
	}
}
/// <summary>
/// パラメーターの計算
/// </summary>
void BossChaseAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*インターバルが残っていたら早期リターン*/
	if (this->interval != 0)
	{
		this->interval--;
		return;
	}

	/*距離を求める*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//座標
	const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//移動目標
	const VECTOR POSITION_TO_TARGET	= VSub(POSITION, MOVE_TARGET);			//目標から現在の移動目標へのベクトル
	const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//距離

	/*もしボスとプレイヤーの間が定数以上離れていたら欲求値を倍増させる*/
	if (DISTANCE >= this->moveDistance)
	{
		if (_boss.GetAttackComboCount() == 0)
		{
			this->parameter->desireValue = 0;
		}
		else
		{
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}