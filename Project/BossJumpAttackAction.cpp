#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "VECTORtoUseful.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossJumpAttack.h"
#include "BossJumpAttackAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossJumpAttackAction::BossJumpAttackAction()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::JUMP_ATTACK))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_FIX_COUNT	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTAC_ROTATE_PLAY_TIME"])
	, MOVE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTAC_MOVE_PLAY_TIME"])
	, STOP_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_STOP_PLAY_TIME"])
	, STOP_DISTANCE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_STOP_MOVE_DISTANCE"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_INTERVAL"])
	, MIN_ACTION_DISTANCE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["NEAR_DISTANCE"])
	, MAX_ACTION_DISTANCE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["MIDDLE_DISTANCE"])
	, CHECK_STATE		(static_cast<int>(Boss::BossState::NORMAL))
	, TARGET_OFFSET		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_INTERVAL"])
{
	const int ATTACK_TYPE	= static_cast<int>(Boss::AttackType::JUMP_ATTACK);
	this->attack			= new BossJumpAttack(ATTACK_TYPE);
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::SLASH_2);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];

}

/// <summary>
/// デストラクタ
/// </summary>
BossJumpAttackAction::~BossJumpAttackAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossJumpAttackAction::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->frameCount			 = 0;
	this->nowTotalAnimPlayTime	 = 0.0f;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossJumpAttackAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*初期化されていなかったら初期化*/
	if (!this->isInitialize)
	{
		//攻撃タイプの設定
		_boss.SetAttackType(Boss::AttackType::JUMP_ATTACK);
		//攻撃フラグを立てる
		this->attack->OnIsStart();
		//移動目標の設定
		const VECTOR TARGET				= player.GetRigidbody().GetPosition();							//目標
		const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();							//座標
			  VECTOR toTarget			= VSub(POSITION, TARGET);									//座標と移動目標間のベクトル
			  VECTOR targetOffsetVector = VNorm(VCross(VNorm(toTarget), Gori::UP_VEC));		//目標オフセットベクトル
		this->moveTarget = VAdd(TARGET, VScale(targetOffsetVector, this->TARGET_OFFSET));
		//初期化フラグを立てる
		this->isInitialize = true;
	}

	/*ヒットストップ*/
	//攻撃が当たっていたら
	if (this->attack->GetIsHitAttack())
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
		//ヒットストップの設定
		this->hitStop->SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
		//攻撃ヒットフラグを下す
		this->attack->OffIsHitAttack();
	}
	//ヒットストップをしていたら早期リターン
	if (this->hitStop->IsHitStop()) return;

	/*フレームカウントが回転定数以上だったら許可フラグを立てる*/
	this->frameCount++;
	if (this->frameCount > this->ROTATE_FIX_COUNT)
	{
		this->isAllowAction = true;
	}

	/*移動処理*/
	VECTOR position				 = _boss.GetRigidbody().GetPosition();		//座標
	VECTOR nowRotation			 = _boss.GetRigidbody().GetRotation();		//回転率
	VECTOR positonToTargetVector = VSub(position, this->moveTarget);	//座標と移動目標間のベクトル
	VECTOR direction			 = VGet(0.0f, 0.0f, 0.0f);				//向き
	float  speed				 = 0.0f;									//移動スピードの設定
	bool   isRotation			 = false;									//回転するか
	bool   isMove				 = false;									//移動するか
	//再生時間が定数範囲内だったらフラグを立てる
	if (this->nowTotalAnimPlayTime > this->MOVE_PLAY_TIME && this->nowTotalAnimPlayTime < this->STOP_PLAY_TIME)
	{
		isMove = true;
	}
	//再生時間が定数以内なら回転フラグを立てる
	if (this->nowTotalAnimPlayTime < this->ROTATE_PLAY_TIME)
	{
		isRotation = true;
		this->moveTarget = player.GetRigidbody().GetPosition();
	}
	//回転処理
	if (isRotation)
	{
		//回転率を補完する
		nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, this->ROTATE_LERP_VALUE);
		//回転率を設定
		_boss.SetRotation(nowRotation);
	}
	//アクション許可フラグが立っていなければ早期リターン
	if (!this->isAllowAction)return;
	//一度も一定距離に近づいていない && 移動フラグが立っていたら
	if (isMove)
	{
		//座標と移動目標との距離を求める
		const float DISTANCE = VSize(positonToTargetVector);
		//距離が定数以上なら速度を設定する
		if (DISTANCE >= this->STOP_DISTANCE)
		{
			speed = this->speed;
		}
		//一定未満ならフラグを立てる
		else
		{
			this->isClose = true;
		}
	}
	//移動ベクトルの設定
	//移動目標の設定
	_boss.SetNowMoveTarget(this->moveTarget);
	//新しい移動ベクトルの算出
	VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
	//移動ベクトルの設定
	_boss.SetVelocity(newVelocity);

	/*アニメーション処理*/
	//アニメーションの設定
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::JUMP_ATTACK));
	//再生時間の設定
	float playTime = this->animationPlayTime;
	this->nowTotalAnimPlayTime += playTime;
	_boss.SetAnimationPlayTime(playTime);
	//アニメーションの再生
	_boss.PlayAnimation();

	/*攻撃判定の更新*/
	this->attack->Update(this->nowTotalAnimPlayTime);

	/*終了判定*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize			= false;
		this->isClose				= false;
		this->nowTotalAnimPlayTime	= 0.0f;
		OffIsSelect(this->MAX_INTERVAL);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossJumpAttackAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	
	/*状態がNORMALだったら欲求値を増加する*/
	if (_boss.GetAngryState() >= this->CHECK_STATE)
	{
		/*距離を求める*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//座標
		const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//移動目標
		const VECTOR TO_TARGET	 = VSub(POSITION, MOVE_TARGET);	//目標から現在の移動目標へのベクトル
		const float  DISTANCE	 = VSize(TO_TARGET);			//距離

		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (this->MIN_ACTION_DISTANCE <= DISTANCE && DISTANCE <= this->MAX_ACTION_DISTANCE)
		{
			//もしコンボが０だったら早期リターン
			if (_boss.GetAttackComboCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}

}
