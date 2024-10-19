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
#include "BossSlashCombo2Attack.h"
#include "BossSlashCombo2Action.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossSlashCombo2Action::BossSlashCombo2Action()
	:isClose(false)
{
	this->attack = new BossSlashCombo2Attack(static_cast<int>(BossAttack::AttackType::SLASH_COMBO_2));
}

/// <summary>
/// デストラクタ
/// </summary>
BossSlashCombo2Action::~BossSlashCombo2Action()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossSlashCombo2Action::Initialize()
{
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
void BossSlashCombo2Action::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*アニメーションの設定*/
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::SLASH_COMBO_2));

	/*攻撃タイプの設定*/
	const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::STAB);
	_boss.SetAttackType(Boss::AttackType::SLASH_COMBO_2);

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*使用する値の準備*/
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();	//座標
	const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();	//移動目標
		  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();	//回転率
		  VECTOR positonToTargetVector  = VSub(POSITION, MOVE_TARGET);			//座標と移動目標間のベクトル
		  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);				//向き


	/*移動ベクトルの設定*/
	 _boss.SetNowMoveTarget(MOVE_TARGET);

	/*ヒットストップの更新*/
	if (this->attack->GetIsHitAttack())
	{
		//auto& effect = Singleton<EffectManager>::GetInstance();
		//effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);

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

	/*回転処理*/
	{
		//プレイヤーから自分の座標までのベクトルを出す
		//アークタンジェントを使って角度を求める
		nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
		//回転率を代入
		_boss.SetRotation(nowRotation);
	}

	/*初期化されていなかったら*/
	if (!this->isInitialize)
	{
		//エフェクトを立てる
		//effect.OnIsBossSlashEffect();
		//攻撃フラグを立てる
		this->attack->OnIsStart();
		this->isInitialize = true;
	}

	/*カウントの計測*/
	bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_SLOW_FRAME_COUNT"]);

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
	//一度でも近づいていなかったら
	if (!this->isClose)
	{
		//カウントが終了していたら
		if (!isEndCount)
		{
			//座標と移動目標との距離を求める
			const float DISTANCE = VSize(positonToTargetVector);
			//距離が定数以上か
			if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_STOP_MOVE_DISTANCE"])
			{
				speed = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_MOVE_SPEED"];
			}
			else
			{
				this->isClose = true;
			}
		}
	}

	/*移動ベクトルを出す*/
	{
		//回転率をもとに移動ベクトルを出す
		direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
		//移動ベクトルを正規化
		direction = VNorm(direction);
		//新しい移動ベクトルを出す（重力を考慮して、Y成分のみ前のものを使用する）
		VECTOR aimVelocity  = VScale(direction, speed);							 //算出された移動ベクトル
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				 //前の移動ベクトル
		VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//新しい移動ベクトル
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
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_2_INTERVAL"]);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossSlashCombo2Action::CalcParameter(const Boss& _boss)
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
	else if (_boss.GetNowPhase() >= static_cast<int>(Boss::Phase::PHASE_8))
	{
		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE <= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::SLASH_COMBO_2)])
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