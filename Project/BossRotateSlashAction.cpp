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
#include "BossRotateSlashAttack.h"
#include "BossRotateSlashAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossRotateSlashAction::BossRotateSlashAction()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::ROTATE_SLASH))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_SLASH_ROTATE_PLAY_TIME"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_SLASH_INTERVAL"])
	, ACTION_DISTANCE	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][this->ATTACK_TYPE])
	, CHECK_STATE		(static_cast<int>(Boss::BossState::ANGRY))

{
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->attack			= new BossRotateSlashAttack(ATTACK_TYPE);
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::ROTATE_SLASH);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
}

/// <summary>
/// デストラクタ
/// </summary>
BossRotateSlashAction::~BossRotateSlashAction()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossRotateSlashAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->isAllowAction			 = false;
	this->frameCount			 = 0;
	this->nowTotalAnimPlayTime	 = 0.0f;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRotateSlashAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*初期化されていなかったら初期化*/
	if (!this->isInitialize)
	{
		//攻撃タイプの設定
		_boss.SetAttackType(Boss::AttackType::SLASH_1);
		//攻撃フラグを立てる
		this->attack->OnIsStart();
		//初期化フラグを立てる
		this->isInitialize = true;
	}

	/*ヒットストップの更新*/
	//攻撃が当たっていたら
	if (this->attack->GetIsHitAttack())
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
		//ヒットストップの設定
		_boss.SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
		//攻撃ヒットフラグを下す
		this->attack->OffIsHitAttack();
	}

	/*移動処理*/
	//使用する値の準備
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();	//座標
	const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();	//移動目標
		  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();	//回転率
		  VECTOR positonToTargetVector	= VSub(POSITION, MOVE_TARGET);	//座標と移動目標間のベクトル
		  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);			//向き
		  float  speed					= 0.0f;									//移動スピードの設定
		  bool   isRotation				= false;								//回転するか
	//この攻撃は最初から攻撃する
	this->isAllowAction = true;
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
	//移動ベクトルの設定
	_boss.SetNowMoveTarget(this->moveTarget);
	//新しい移動ベクトルの算出
	VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
	//移動ベクトルの設定
	_boss.SetVelocity(newVelocity);

	/*アニメーション処理*/
	this->nowTotalAnimPlayTime += this->animationPlayTime;
	//アニメーションの設定
	_boss.SetNowAnimation(this->nextAnimation);
	//再生時間の設定
	_boss.SetAnimationPlayTime(this->animationPlayTime);
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
void BossRotateSlashAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*インターバルが残っていたら早期リターン*/
	if (this->interval != 0)
	{
		this->interval--;
		return;
	}

	/*状態がNORMAL,ANGRYだったら欲求値を増加する*/
	if (_boss.GetBossState() == this->CHECK_STATE)
	{
		/*距離を求める*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//座標
		const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//移動目標
		const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);			//目標から現在の移動目標へのベクトル
		const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//距離

		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE <= this->ACTION_DISTANCE)
		{
			//コンボが０だったら早期リターン
			if (_boss.GetAttackCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}