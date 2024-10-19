#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossJumpAttack.h"
#include "BossJumpAttackAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossJumpAttackAction::BossJumpAttackAction()
	: isClose(false)
{
	this->attack = new BossJumpAttack(static_cast<int>(BossAttack::AttackType::JUMP_ATTACK));
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

	/*アニメーションの設定*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::JUMP_ATTACK));

	/*攻撃タイプの設定*/
	const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::JUMP_ATTACK);
	_boss.SetAttackType(Boss::AttackType::JUMP_ATTACK);

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*使用する値の準備*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//座標
	const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//移動目標
	VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//回転率
	VECTOR positonToTargetVector = VSub(POSITION, MOVE_TARGET);			//座標と移動目標間のベクトル
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);				//向き

/*移動ベクトルの設定*/
	_boss.SetNowMoveTarget(MOVE_TARGET);

	/*ヒットストップの更新*/
	if (this->attack->GetIsHitAttack())
	{
		this->hitStop->SetHitStop
		(
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE],
			static_cast<int>(HitStop::Type::STOP),
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE],
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE]
		);
		this->attack->OffIsHitAttack();
	}
	if (this->hitStop->IsHitStop()) return;

	/*初期化されていなかったら*/
	if (!this->isInitialize)
	{
		//エフェクトを立てる
		//effect.();
		//攻撃フラグを立てる
		this->attack->OnIsStart();
		this->isInitialize = true;
	}

	/*カウントの計測*/
	this->frameCount++;
	//プレイヤーを追跡するか
	bool isHoming = (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_HOMING_FRAME"]);
	//移動するか
	bool isMove = (this->frameCount >= json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_START_MOVE_FRAME"]);
	//停止フレームを超えていたら移動フラグを下す
	if (isMove && (this->frameCount >= json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_STOP_MOVE_FRAME"]))
	{
		isMove = false;
	}

	/*回転処理*/
	if (isHoming)
	{
		//プレイヤーから自分の座標までのベクトルを出す
		//アークタンジェントを使って角度を求める
		nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
		//回転率を代入
		_boss.SetRotation(nowRotation);
	}

	/*アニメーション処理*/
	{
		//アニメーション再生時間
		float animationPlayTime = _boss.GetAnimationPlayTime();
		_boss.SetAnimationPlayTime(animationPlayTime);
		/*アニメーションの再生*/
		_boss.PlayAnimation();
	}

	/*移動スピードの設定*/
	float speed = 0.0f;
	//カウントが終了していたら
	if (isMove)
	{
		//座標と移動目標との距離を求める
		const float DISTANCE = VSize(positonToTargetVector);
		//距離が定数以上か
		if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_STOP_MOVE_DISTANCE"])
		{
			speed = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_MOVE_SPEED"];
		}
	}

	/*移動ベクトルを出す*/
	{
		//回転率をもとに移動ベクトルを出す
		direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
		//移動ベクトルを正規化
		direction = VNorm(direction);
		//新しい移動ベクトルを出す（重力を考慮して、Y成分のみ前のものを使用する）
		VECTOR aimVelocity = VScale(direction, speed);							 //算出された移動ベクトル
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				 //前の移動ベクトル
		VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//新しい移動ベクトル
		//移動ベクトルの設定
		_boss.SetVelocity(newVelocity);
	}

	/*攻撃判定の更新*/
	this->attack->Update();

	/*終了判定*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize = false;
		this->isClose = false;
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_INTERVAL"]);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossJumpAttackAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*距離を求める*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//座標
	const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//移動目標
	const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);	//目標から現在の移動目標へのベクトル
	const float  DISTANCE = VSize(POSITION_TO_TARGET);			//距離

	this->parameter->desireValue = 0;
	
	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		return;
	}

	/*Phaseが8以上だったら欲求値を増加する*/
	else if (_boss.GetNowPhase() >= static_cast<int>(Boss::Phase::PHASE_5))
	{
		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::JUMP_ATTACK)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			if (_boss.GetAttackComboCount() == 0)
			{
				this->parameter->desireValue = 1;
			}
			else
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}

}
