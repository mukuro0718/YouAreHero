#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossRoarAction.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossRoarAction::BossRoarAction()
	: prevState				(0)
	, isFinishedFirstRoar	(false)
	, isInitializeColorScale(false)
	, checkedState(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->maxDesireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->checkedState = static_cast<int>(Boss::BossState::ANGRY);
}

/// <summary>
/// デストラクタ
/// </summary>
BossRoarAction::~BossRoarAction()
{
}
/// <summary>
/// 初期化
/// </summary>
void BossRoarAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isInitializeColorScale = false;
	this->isFinishedFirstRoar	 = false;
	this->isPriority			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = this->maxDesireValue;
	this->parameter->interval	 = 0;
	this->prevState				 = 1;
	this->frameTime				 = 0;
	this->nextAnimation = static_cast<int>(Boss::AnimationType::ROAR);
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRoarAction::Update(Boss& _boss)
{
	//int startTime = GetNowCount();

	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	///*カラースケールの処理*/
	//{
	//	//税所の咆哮では色を変えない
	//	if (this->isFinishedFirstRoar)
	//	{
	//		//ボスのモデルハンドル
	//		const int MODEL_HANDLE = _boss.GetModelHandle();
	//		//初期化フラグが立っていなかったらカラースケールの初期化
	//		if (!this->isInitializeColorScale)
	//		{
	//			//初期化フラグを立てる
	//			this->isInitializeColorScale = true;
	//			//現在の色を取得
	//			for (int i = 0; i < this->baseColorScale.size(); i++)
	//			{
	//				this->getColorScaleMap[i](this->baseColorScale[i], this->nowColorScale[i], MODEL_HANDLE);
	//			}
	//		}
	//		//カラースケールの更新(ここでは赤色になるようにする)
	//		for (int i = 0; i < this->nowColorScale.size(); i++)
	//		{
	//			const COLOR_F TARGET = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["ROAR_TARGET_COLOR_SCALE"]);
	//			const COLOR_F LERP = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["LERP_COLOR_SCALE"]);
	//			this->nowColorScale[i] = LerpColor(this->nowColorScale[i], TARGET, LERP);
	//			this->setColorScaleMap[i](this->nowColorScale[i], MODEL_HANDLE);
	//		}
	//	}
	//}

	/*この処理の開始時に一度だけ呼ぶ*/
	if (this->frameCount == 0)
	{
		auto& effect = Singleton<EffectManager>::GetInstance();
		effect.OnIsEffect(EffectManager::EffectType::BOSS_ROAR);
		this->frameCount++;
		//怒り状態を合わせる
		this->prevState = _boss.GetAngryState();
		//選択されていたら欲求値を０にする
		this->parameter->desireValue = 0;
		//アニメーションの設定
		_boss.SetNowAnimation(this->nextAnimation);
		//アニメーション再生時間の設定
		_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());
		//スピードを０にする
		float speed = 0.0f;
		_boss.SetSpeed(speed);
		//回転率をもとに、移動する向きを出す
		VECTOR	rotation  = _boss.GetRigidbody().GetRotation();					//回転率
		VECTOR	direction = VGet(0.0f, 0.0f, 0.0f);								//向き
				direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
				direction = VNorm(direction);
		//移動ベクトルを出す（重力を加算するため、Yベクトルのみ前のベクトルを使用する）
		VECTOR aimVelocity  = VScale(direction, speed);								//算出された移動ベクトル
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//前の移動ベクトル
		VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//新しい移動ベクトル
		//移動ベクトルの設定
		_boss.SetVelocity(newVelocity);
	}
	
	/*アニメーションの再生*/
	_boss.PlayAnimation();

	/*咆哮中にアニメーションが終了していたら、選択フラグを下してフェーズを統一する*/
	if (this->isSelect && _boss.GetIsChangeAnimation())
	{
		OffIsSelect(0);
		this->isFinishedFirstRoar = true;
		this->isInitializeColorScale = false;
		this->frameCount = 0;
	}
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossRoarAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*もしHPが０以下だったら欲求値を０にして早期リターン*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*一度も咆哮をしていなければ咆哮フラグと優先フラグを立てる*/
	if (!this->isFinishedFirstRoar)
	{
		this->parameter->desireValue = this->maxDesireValue;
		this->isPriority = true;
	}

	/*AngryStateTypeがANGRYになったら咆哮をする*/
	int nowState = _boss.GetAngryState();
	if (nowState != this->prevState)
	{
		if (nowState == this->checkedState)
		{
			this->parameter->desireValue = this->maxDesireValue;
			this->isPriority = true;
		}
		else
		{
			this->prevState = nowState;
		}
	}
}

