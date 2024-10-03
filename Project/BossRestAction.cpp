#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "Boss.h"
#include "BossRestAction.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossRestAction::BossRestAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
BossRestAction::~BossRestAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossRestAction::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isSetMoveDirection	 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->maxFrame				 = GetRand(json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]);
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRestAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*選択されていたら欲求値を０にする*/
	this->parameter->desireValue = 0;

	/*移動する方向が設定されていなければ*/
	if (!this->isSetMoveDirection)
	{
		//移動方向の設定（RIGHT=1）
		this->directionType = GetRand(static_cast<int>(DirectionType::RIGHT));
		//フラグを立てる
		this->isSetMoveDirection = true;
	}

	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*使用する値の準備*/
	const float  SPEED		= json.GetJson(JsonManager::FileType::ENEMY)["SIDE_WALK_SPEED"];
	const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();
		  VECTOR velocity	= VGet(0.0f, 0.0f, 0.0f);			 //向き
		  VECTOR nowRotation = _boss.GetRigidbody().GetRotation();

	/*移動ベクトルの設定*/
	_boss.SetNowMoveTarget(MOVE_TARGET);

	/*プレイヤーから自分の座標までのベクトルを出す*/
	VECTOR positionToTargetVector = VSub(POSITION, MOVE_TARGET);

	/*アークタンジェントを使って角度を求める*/
	nowRotation.y = static_cast<float>(atan2(static_cast<double>(positionToTargetVector.x), static_cast<double>(positionToTargetVector.z)));

	/*回転率を代入*/
	_boss.SetRotation(nowRotation);


	/*スピードを０にする*/
	_boss.SetSpeed(SPEED);

	/*回転率をもとに、移動する向きを出す*/
	if (this->directionType == static_cast<int>(DirectionType::RIGHT))
	{
		velocity = VCross(positionToTargetVector, Gori::UP_VEC);
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::WALK_RIGHT));
	}
	else
	{
		velocity = VCross(positionToTargetVector, Gori::UP_VEC);
		velocity = VScale(velocity, -1.0f);
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::WALK_LEFT));
	}


	/*向きベクトルを正規化*/
	velocity = VNorm(velocity);

	/*移動ベクトルを出す（重力を加算するため、Yベクトルのみ前のベクトルを使用する）*/
	VECTOR aimVelocity = VScale(velocity, SPEED);						//算出された移動ベクトル
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//前の移動ベクトル
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//新しい移動ベクトル

	/*移動ベクトルの設定*/
	_boss.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	_boss.PlayAnimation();

	//フレーム計測
	bool isEndCount = FrameCount(this->maxFrame);
	//フレーム計測が終了していたら
	if (isEndCount)
	{
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["REST_INTERVAL"]);
		this->maxFrame = GetRand(json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]);
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRestAction::CalcParameter(const Boss& _boss)
{
	/*インターバルが残っていたら欲求値を0にする*/
	if (this->parameter->interval != 0)
	{
		this->parameter->desireValue = 0;
		this->parameter->interval--;
		return;
	}

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	else if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		this->parameter->desireValue = 0;
	}

	else
	{
		this->parameter->desireValue++;
	}
}