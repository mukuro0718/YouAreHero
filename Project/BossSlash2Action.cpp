#include <DxLib.h>
#include "EffekseerForDXLib.h"
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
#include "HitStop.h"
#include "BossAttack.h"
#include "BossSlash2Attack.h"
#include "BossSlash2Action.h"
#include "PlayerManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossSlash2Action::BossSlash2Action()
	: isClose(false)
{
	this->attack = new BossSlash2Attack(static_cast<int>(BossAttack::AttackType::SLASH_2));
}

/// <summary>
/// デストラクタ
/// </summary>
BossSlash2Action::~BossSlash2Action()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossSlash2Action::Initialize()
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
/// 更新
/// </summary>
void BossSlash2Action::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }


	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*攻撃準備*/
	{
		//初期化されていなかったら
		if (!this->isInitialize)
		{
			//攻撃タイプの設定
			_boss.SetAttackType(Boss::AttackType::SLASH_2);
			//攻撃フラグを立てる
			this->attack->OnIsStart();
			//移動目標の設定
			this->moveTarget = player.GetRigidbody().GetPosition();
			//初期化フラグを立てる
			this->isInitialize = true;
		}

	}

	/*ヒットストップ*/
	{
		//ヒットストップの更新
		if (this->attack->GetIsHitAttack())
		{
			const int ATTACK_TYPE = static_cast<int>(Boss::AttackType::SLASH_2);
			//ヒットストップの設定
			this->hitStop->SetHitStop
			(
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][ATTACK_TYPE],
				static_cast<int>(HitStop::Type::STOP),
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][ATTACK_TYPE],
				json.GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][ATTACK_TYPE]
			);
			//攻撃ヒットフラグを下す
			this->attack->OffIsHitAttack();
		}
		/*ヒットストップをしていたら早期リターン*/
		if (this->hitStop->IsHitStop()) return;
	}

	/*カウントの計測*/
	this->frameCount++;

	/*移動処理*/
	{
		/*使用する値の準備*/
		const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//座標
		const VECTOR LERP_VALUE = Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);//回転率の補完値
		VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//回転率
		VECTOR direction = VGet(0.0f, 0.0f, 0.0f);			//向き
		float  speed = 0.0f;									//移動の速さ
		bool	 isRotation = false;								//回転するか
		bool   isMove = false;								//移動するか

		{
			//フレームカウントが回転定数未満だったら許可フラグを立てる
			if (this->frameCount > json.GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
			{
				isAllowAction = true;
			}
			//フレームカウントが定数以内なら移動フラグを立てる
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_MOVE_FRAME"])
			{
				isMove = true;
			}
			//フレームカウントが定数以内なら回転フラグを立てる
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_ROTATE_FRAME"])
			{
				isRotation = true;
			}
			//フレームカウントが定数以内なら移動目標を更新する
			if (this->frameCount < json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_HOMING_FRAME"])
			{
				this->moveTarget = player.GetRigidbody().GetPosition();
			}
		}
		//回転処理
		{
			//回転フラグが立っていたら
			if (isRotation)
			{
				//座標と移動目標間のベクトル
				VECTOR positonToTargetVector = VSub(POSITION, this->moveTarget);
				//回転率を補完する
				nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, LERP_VALUE);
				//回転率を代入
				_boss.SetRotation(nowRotation);
			}
		}
		////アクション許可フラグが立っていなければ早期リターン
		//if (!this->isAllowAction)return;
		//移動スピードの設定
		{
			//一度でも近づいていない＆＆移動フラグが立っていたら
			if (!this->isClose && isMove)
			{
				//座標と移動目標間のベクトル
				VECTOR positonToTargetVector = VSub(POSITION, this->moveTarget);
				//座標と移動目標との距離を求める
				const float DISTANCE = VSize(positonToTargetVector);
				//距離が定数以上だったら移動速度を代入する
				if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_STOP_MOVE_DISTANCE"])
				{
					speed = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_MOVE_SPEED"];
				}
				//距離が未満だったらフラグを立てる
				else
				{
					this->isClose = true;
				}
			}
		}
		//移動ベクトルの設定
		{
			//移動目標の設定
			_boss.SetNowMoveTarget(this->moveTarget);
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
	}






	/*アニメーション処理*/
	{
		//アニメーションの設定
		_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::SLASH_2));
		//アニメーション再生時間
		float animationPlayTime = _boss.GetAnimationPlayTime();
		_boss.SetAnimationPlayTime(animationPlayTime);
		//アニメーションの再生
		_boss.PlayAnimation();
	}

	/*攻撃判定の更新*/
	this->attack->Update();

	/*終了判定*/
	{
		//アニメーションが終了していたら
		if (_boss.GetIsChangeAnimation())
		{
			//各フラグを下す
			this->isInitialize = false;
			this->isClose = false;
			//その他変数の初期化とインターバルのセット
			OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_INTERVAL"]);
			//コンボ数を減らす
			_boss.DecAttackComboCount();
		}
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossSlash2Action::CalcParameter(const Boss& _boss)
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
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*状態がTIRED,NORMAL,ANGRYだったら欲求値を増加する*/
	else if (_boss.GetAngryState() >= static_cast<int>(Boss::BossState::NORMAL))
	{
		/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
		if (DISTANCE <= json.GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][static_cast<int>(Boss::AttackType::SLASH_2)])
		{
			Boss::AttackType type = _boss.GetPrevAttackType();
			//コンボ数が残っていなかったらリターン
			if (_boss.GetAttackComboCount() == 0)return;
			//前の攻撃がスラッシュコンボ２だったら
			if (type == Boss::AttackType::SLASH_COMBO_2)
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			}
			//それ以外なら
			else
			{
				this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["NORMAL_DESIRE_VALUE"];
			}
		}
	}
}