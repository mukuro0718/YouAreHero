#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossIdleAction.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossIdleAction::BossIdleAction()
	: maxFrameCount(0)
{

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
	this->maxFrameCount			 = 0;
	this->prevAngryState		 = 1;
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossIdleAction::Update(Boss& _boss)
{
	///////////////////////////////
	/*死亡していたらisSelectをfalseにして早期リターン*/
	if (_boss.GetHP() < 0 ) { OffIsSelect(this->maxFrameCount); return; }

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

	/*カラースケールの変更*/
	{
		if (!this->isChangeColorScale && 
			_boss.GetAngryState() == static_cast<int>(Boss::AngryStateType::NORMAL))
		{
			this->isChangeColorScale = true;
		}

		//初期化フラグが立っていたら現在の色を取得する
		if (this->isChangeColorScale)
		{
			//ボスのモデルハンドル
			const int MODEL_HANDLE = _boss.GetModelHandle();
			//フラグが立っていなかったらカラースケールの変更
			if (!this->isInitializeColorScale)
			{
				//カラースケールの更新(ここでは赤色になるようにする)
				for (int i = 0; i < this->nowColorScale.size(); i++)
				{
					this->getColorScaleMap[i](this->baseColorScale[i], this->nowColorScale[i], MODEL_HANDLE);
				}
				this->isInitializeColorScale = true;
			}
			//カラースケールの更新(ここでは赤色になるようにする)
			for (int i = 0; i < this->nowColorScale.size(); i++)
			{
				const COLOR_F TARGET = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["IDLE_TARGET_COLOR_SCALE"]);
				const COLOR_F LERP = ColorConvert(json.GetJson(JsonManager::FileType::ENEMY)["LERP_COLOR_SCALE"]);
				this->nowColorScale[i] = LerpColor(this->nowColorScale[i], TARGET, LERP);
				this->setColorScaleMap[i](this->nowColorScale[i], MODEL_HANDLE);
			}
		}
	}

	/*怒り状態を合わせる*/
	this->prevAngryState = _boss.GetAngryState();

	/*使用する値の準備*/
	const float  SPEED					= json.GetJson(JsonManager::FileType::ENEMY)["SIDE_WALK_SPEED"];					//速度
	const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();													//移動目標
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();													//ボスの座標
	const VECTOR LERP_VALUE				= Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["ROTATE_LERP_VALUE"]);	//回転率の補完値
		  VECTOR velocity				= Gori::ORIGIN;																			//向き
		  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();													//現在の回転率
		  VECTOR nextRotation			= Gori::ORIGIN;																			//次の座標
		  VECTOR positionToTargetVector	= VSub(POSITION,MOVE_TARGET);														//ターゲットから自分の座標までのベクトルを出す

	/*回転処理*/
	{
		//移動目標の設定
		_boss.SetNowMoveTarget(MOVE_TARGET);
		//回転率を保管する
		nowRotation = GetLerpRotation(_boss,positionToTargetVector,nowRotation,LERP_VALUE);
		//回転率を設定
		_boss.SetRotation(nowRotation);
	}
	
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
	_boss.SetAnimationPlayTime(_boss.GetAnimationPlayTime());
	_boss.PlayAnimation();

	//フレーム計測
	bool isEndCount = FrameCount(this->maxFrameCount);
	//フレーム計測が終了していたら
	if (isEndCount)
	{
		OffIsSelect(json.GetJson(JsonManager::FileType::ENEMY)["REST_INTERVAL"]);
		_boss.SetAttackComboCount();
		this->isChangeColorScale = false;
	}
}

/// <summary>
/// パラメーターの計算
/// </summary>
void BossIdleAction::CalcParameter(const Boss& _boss)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*もしHPが０以下だったら欲求値を０にして優先フラグを下す*/
	if (_boss.GetHP() <= 0)return;

	/*ボスのAngryTypeを取得*/
	int nowAngryState = _boss.GetAngryState();

	/*AngryStateがNORMALの時に攻撃コンボが残っていなかったら*/
	if (nowAngryState == static_cast<int>(Boss::AngryStateType::NORMAL))
	{
		if (nowAngryState != this->prevAngryState)
		{
			this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			this->isPriority			 = true;
			this->isChangeColorScale	 = true;
			this->isInitializeColorScale = false;
		}
		//ボスの攻撃が０だったら
		else if (_boss.GetAttackComboCount() == 0)
		{
			this->parameter->desireValue = json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
			this->isPriority = true;
			this->isChangeColorScale = true;
			this->isInitializeColorScale = false;
		}
	}

	/*違う状態なら現在の状態を保存する*/
	else
	{
		this->prevAngryState = _boss.GetAngryState();
	}

	/*ボスのAngryTypeをもとに最大フレームも決めておく*/
	this->maxFrameCount = json.GetJson(JsonManager::FileType::ENEMY)["IDLE_ACTION_MAX_FRAME"];
}