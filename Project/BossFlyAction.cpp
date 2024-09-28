#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Boss.h"
#include "BossFlyAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "BossAttackManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossFlyAction::BossFlyAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
BossFlyAction::~BossFlyAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossFlyAction::Initialize()
{
	this->isSelect = false;
	this->isInitialize = false;
	this->frameCount = 0;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossFlyAction::Update(Boss& _boss)
{
	/*選択されていたら*/
	if (this->isSelect)
	{
		/*シングルトンクラスのインスタンスの取得*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		auto& effect = Singleton<EffectManager>::GetInstance();
		auto& attack = Singleton<BossAttackManager>::GetInstance();
		auto& json = Singleton<JsonManager>::GetInstance();


		/*初期化されていなかったら*/
		if (!this->isInitialize)
		{
			//エフェクトを立てる
			//effect.();
			//攻撃フラグを立てる
			attack.OnIsStart(static_cast<int>(BossAttackManager::AttackType::FLY_ATTACK));
			this->isInitialize = true;

			/*一度だけ移動処理*/
			//使用する変数の準備
			const VECTOR POSITION = _boss.GetRigidbody().GetPosition(); //座標
			const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();//移動目標
			VECTOR nowRotation = _boss.GetRigidbody().GetRotation(); //回転率
			VECTOR positonToTargetVector = VSub(POSITION, MOVE_TARGET); //座標と移動目標間のベクトル

			//移動ベクトルの設定
			_boss.SetNowMoveTarget(MOVE_TARGET);
			//回転処理
			{
				//プレイヤーから自分の座標までのベクトルを出す
				//アークタンジェントを使って角度を求める
				nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
				//回転率を代入
				_boss.SetRotation(nowRotation);
			}
			//移動スピードの設定
			_boss.SetSpeed(0.0f);
		}

		/*カウントの計測*/
		bool isEndCount = FrameCount(json.GetJson(JsonManager::FileType::ENEMY)["FLY_ATTACK_SLOW_FRAME_COUNT"]);

		//移動ベクトルを出す
		{
			VECTOR nowRotation = _boss.GetRigidbody().GetRotation(); //回転率
			VECTOR direction = VGet(0.0f, 0.0f, 0.0f);
			float speed = _boss.GetSpeed();

			//スピードを加速
			speed += static_cast<float>(json.GetJson(JsonManager::FileType::ENEMY)["FLY_ADD_MOVE_SPEED"]);
			//スピードの設定
			_boss.SetSpeed(speed);
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

		/*アニメーション再生時間の設定*/
		{
			float animationPlayTime = _boss.GetAnimationPlayTime();
			//カウントが終了していなければ
			/*if (isEndCount)
			{
				animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["FLY_SLOW_PLAY_TIME"];
			}*/
			_boss.SetAnimationPlayTime(animationPlayTime);
		}

		/*終了判定*/
		if (_boss.GetIsChangeAnimation())
		{
			this->isInitialize = false;
			OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["FLY_INTERVAL"]);
		}
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossFlyAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	const VECTOR POSITION					= _boss.GetRigidbody().GetPosition();
	const VECTOR TARGET						= player.GetRigidbody().GetPosition();
	const VECTOR POSITION_TO_TARGET_VECTOR	= VSub(POSITION, TARGET); //座標と移動目標間のベクトル
	const float  DISTANCE					= VSize(POSITION_TO_TARGET_VECTOR);
	/*追加する欲求値*/
	int addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE;

	/*HPが０以下またはフェーズが異なっていたら欲求値を0にする*/
	if ((_boss.GetHP() <= 0) || (_boss.GetNowPhase() != _boss.GetPrevPhase()))
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*インターバルが残っていたら欲求値を０にする*/
	else if (!this->parameter->CalcInterval())
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*もしボスとプレイヤーの間が定数以内なら欲求値を倍増させる*/
	else if (DISTANCE >= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"])
	{
		addDesireValue = this->parameter->BASE_ADD_DESIRE_VALUE * 2.0f;
	}
	else
	{
		addDesireValue = -this->parameter->MAX_PARAMETER;
	}

	/*欲求値を増加させる*/
	this->parameter->AddDesireValue(addDesireValue);
}
