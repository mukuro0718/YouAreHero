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
#include "BossIdleAction.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossIdleAction::BossIdleAction()
	: maxFrameCount(0)
	, NORMAL_MAX_ANGRY_FRAME_COUNT(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["NORMAL_IDLE_ACTION_MAX_FRAME"])
	, ANGRY_MAX_ANGRY_FRAME_COUNT(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ANGRY_IDLE_ACTION_MAX_FRAME"])
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->speed				= 0;
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["ROTATE_LERP_VALUE"]);
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::IDLE);
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["REST_INTERVAL"];
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->checkedState		= static_cast<int>(Boss::BossState::NORMAL);
}

/// <summary>
/// デストラクタ
/// </summary>
BossIdleAction::~BossIdleAction()
{

}
/// <summary>
/// 初期化
/// </summary>
void BossIdleAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isPriority			 = false;
	this->isInitializeColorScale = false;
	this->isSetMoveDirection	 = false;
	this->isChangeColorScale	 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->prevState				 = 1;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossIdleAction::Update(Boss& _boss)
{
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0 ) { OffIsSelect(this->maxFrameCount); return; }

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*カラースケールの変更*/
	//{
	//	if (!this->isChangeColorScale && 
	//		_boss.GetAngryState() == static_cast<int>(Boss::BossState::NORMAL))
	//	{
	//		this->isChangeColorScale = true;
	//	}

	//	//初期化フラグが立っていたら現在の色を取得する
	//	if (this->isChangeColorScale)
	//	{
	//		//ボスのモデルハンドル
	//		const int MODEL_HANDLE = _boss.GetModelHandle();
	//		//フラグが立っていなかったらカラースケールの変更
	//		if (!this->isInitializeColorScale)
	//		{
	//			//カラースケールの更新(ここでは赤色になるようにする)
	//			for (int i = 0; i < this->nowColorScale.size(); i++)
	//			{
	//				this->getColorScaleMap[i](this->baseColorScale[i], this->nowColorScale[i], MODEL_HANDLE);
	//			}
	//			this->isInitializeColorScale = true;
	//		}
	//		//カラースケールの更新(ここでは赤色になるようにする)
	//		for (int i = 0; i < this->nowColorScale.size(); i++)
	//		{
	//			const COLOR_F TARGET = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["IDLE_TARGET_COLOR_SCALE"]);
	//			const COLOR_F LERP = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["LERP_COLOR_SCALE"]);
	//			this->nowColorScale[i] = LerpColor(this->nowColorScale[i], TARGET, LERP);
	//			this->setColorScaleMap[i](this->nowColorScale[i], MODEL_HANDLE);
	//		}
	//	}
	//}

	//移動ベクトルを出す（重力を加算するため、Yベクトルのみ前のベクトルを使用する）
	VECTOR aimVelocity  = VScale(Gori::ORIGIN, this->speed);						//算出された移動ベクトル
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//前の移動ベクトル
	VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//新しい移動ベクトル
	//移動ベクトルの設定
	_boss.SetVelocity(newVelocity);

	/*アニメーションの再生*/
	_boss.SetNowAnimation(this->nextAnimation);
	_boss.SetAnimationPlayTime(this->animationPlayTime);
	_boss.PlayAnimation();

	//フレーム計測
	bool isEndCount = FrameCount(this->maxFrameCount);
	//フレーム計測が終了していたら
	if (isEndCount)
	{
		OffIsSelect(this->maxInterval);
		_boss.SetAttackComboCount();
		this->isChangeColorScale = false;
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossIdleAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*もしHPが０以下だったら欲求値を０にして優先フラグを下す*/
	if (_boss.GetHP() <= 0)return;

	/*攻撃コンボがなければ*/
	if (_boss.GetAttackComboCount() == 0)
	{
		int nowAngryState = _boss.GetAngryState();
		if (nowAngryState == static_cast<int>(Boss::BossState::NORMAL))
		{
			this->parameter->desireValue = this->maxDesireValue;
			this->isPriority			 = true;
			this->isChangeColorScale	 = true;
			this->isInitializeColorScale = false;
		}
	}
}