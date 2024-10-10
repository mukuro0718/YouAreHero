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
#include "BossMeleeAttack.h"
#include "BossMeleeAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossMeleeAction::BossMeleeAction()
{
	this->attack = new BossMeleeAttack(static_cast<int>(BossAttack::AttackType::MELEE));
}

/// <summary>
/// デストラクタ
/// </summary>
BossMeleeAction::~BossMeleeAction()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossMeleeAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
	this->hitStop->Initialize();
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossMeleeAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*アニメーションの設定*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::MELEE));

	/*攻撃タイプの設定*/
	_boss.SetAttackType(Boss::AttackType::MELEE);

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*使用する値の準備*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition(); //座標
	const VECTOR NEXT_MOVE_TARGET = player.GetRigidbody().GetPosition();
	VECTOR nowMoveTarget = _boss.GetNowMoveTarget();//移動目標
	VECTOR nowRotation = _boss.GetRigidbody().GetRotation(); //回転率
	VECTOR positonToTargetVector = VGet(0.0f, 0.0f, 0.0f); //プレイヤーから自分の座標までのベクトル
	VECTOR direction = VGet(0.0f, 0.0f, 0.0f);

	/*ヒットストップの更新*/
	if (this->attack->GetIsHitAttack())
	{
		//auto& effect = Singleton<EffectManager>::GetInstance();
		//effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);

		this->hitStop->SetHitStop
		(
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][static_cast<int>(BossAttack::AttackType::MELEE)],
			static_cast<int>(HitStop::Type::STOP),
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][static_cast<int>(BossAttack::AttackType::MELEE)],
			json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][static_cast<int>(BossAttack::AttackType::MELEE)]
		);
		this->attack->OffIsHitAttack();
	}
	if (this->hitStop->IsHitStop()) return;

	/*初期化されていなかったら*/
	if (!this->isInitialize)
	{
		//エフェクトを立てる
		//effect.OnIsBossSlashEffect();
		//攻撃フラグを立てる
		this->attack->OnIsStart();
		this->isInitialize = true;
		_boss.SetAnimationPlayTime(0.0f);
		//移動ベクトルの設定
		_boss.SetNowMoveTarget(NEXT_MOVE_TARGET);
		//プレイヤーから自分の座標までのベクトルを出す
		positonToTargetVector = VSub(POSITION, NEXT_MOVE_TARGET);

		/*回転処理*/
		//アークタンジェントを使って角度を求める
		nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
		//回転率を代入
		_boss.SetRotation(nowRotation);

	}

	/*アニメーション再生時間の設定*/
	float animationPlayTime = _boss.GetAnimationPlayTime();
	_boss.SetAnimationPlayTime(animationPlayTime);

	/*アニメーションの再生*/
	_boss.PlayAnimation();

	/*移動スピードの設定*/
	float speed = 0.0f;

	/*移動ベクトルを出す*/
	{
		//回転率をもとに移動ベクトルを出す
		direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
		//移動ベクトルを正規化
		direction = VNorm(direction);
		//新しい移動ベクトルを出す（重力を考慮して、Y成分のみ前のものを使用する）
		VECTOR aimVelocity = VScale(direction, speed);					 //算出された移動ベクトル
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
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_INTERVAL"]);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossMeleeAction::CalcParameter(const Boss& _boss)
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

	/*Phaseが1以上だったら欲求値を増加する*/
	else if (_boss.GetNowPhase() >= static_cast<int>(Boss::Phase::PHASE_2))
	{
		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE <= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::MELEE)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			if (_boss.GetAttackComboCount() == 0)
			{
				this->parameter->desireValue = 1;
			}
			else if (type == Boss::AttackType::SLASH_COMBO_2)
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			}
			else
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}
}