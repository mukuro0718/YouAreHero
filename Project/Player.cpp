#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Animation.h"
#include "Model.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "Player.h"
#include "CameraManager.h"
#include "EnemyManager.h"
#include "Vector4.h"
#include "Collider.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: model				(nullptr)
	, collider			(nullptr)
	, moveVector		{ 0.0f, 0.0f, 0.0f }
	, direction			{ 0.0f, 0.0f, 0.0f }
	, fixVector			{ 0.0f, 0.0f, 0.0f }
	, moveVectorRotation{ 0.0f, 0.0f, 0.0f }
	, wasd				{ 0.0f, 0.0f, 0.0f }
	, lStick			{ 0.0f, 0.0f, 0.0f }
	, velocity			(0.0f)
	, attackComboCount  (0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*インスタンスの作成*/
	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::PLAYER));
	this->state = new BitFlag();
	this->collider = new Collider();

	/*vectorの追加*/
	for (int i = 0; i < this->COUNT_NUM; i++)
	{
		this->frameCount.emplace_back(0);
		this->isCount.emplace_back(false);
	}

	/*アニメーションの設定*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::STANDING_IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);
	this->attackAnimationMap.emplace(this->CASTING				, static_cast<int>(AnimationType::CASTING));
	this->attackAnimationMap.emplace(this->COMBO_ATTACK			, static_cast<int>(AnimationType::COMBO));
	this->attackAnimationMap.emplace(this->CROUCH_SLASH			, static_cast<int>(AnimationType::CROUCH_SLASH));
	this->attackAnimationMap.emplace(this->KICK					, static_cast<int>(AnimationType::KICK));
	this->attackAnimationMap.emplace(this->PUNCH				, static_cast<int>(AnimationType::PUNCH));
	this->attackAnimationMap.emplace(this->ROTATION_ATTACK		, static_cast<int>(AnimationType::ROTATION_ATTACK));
	this->attackAnimationMap.emplace(this->SLASH_1				, static_cast<int>(AnimationType::SLASH_1));
	this->attackAnimationMap.emplace(this->SLASH_2				, static_cast<int>(AnimationType::SLASH_2));
	this->attackComboStateMap.emplace(0, this->SLASH_1);
	this->attackComboStateMap.emplace(1, this->SLASH_2);
	this->attackComboStateMap.emplace(2, this->COMBO_ATTACK);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	DeleteMemberInstance(this->collider);
	this->frameCount.clear();
	this->isCount.clear();
}

void Player::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR position = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//座標
	const VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//回転率
	const VECTOR scale = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //拡大率
	this->velocity = 0.0f;
	this->direction = VGet(0.0f, 0.0f, -1.0f);

	/*モデルのトランスフォームの設定*/
	this->model->SetTransform(position, rotation, scale);
}

/// <summary>
/// アクション
/// </summary>
void Player::Action()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*立ち座りの切り替え*/
	StandOrSit();

	/*移動*/
	Move();

	/*ロックオン*/
	LockOn();

	/*ジャンプ*/
	Jump();
	
	/*ガード*/
	Block();

	/*回避*/
	Avoid();

	/*咆哮*/
	Roar();

	/*攻撃*/
	Attack();

	/*なにも移動していなければ待機状態をセットする*/
	if (DontAnyAction())
	{
		this->state->SetFlag(this->IDLE);
	}

	/*コライダーの設定*/
	const VECTOR position = this->model->GetPosition() + this->moveVector;
	const float  radius	  = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];
	this->collider->SetSphere(position, radius);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	VECTOR position = this->model->GetPosition() + this->moveVector;
	const float  radius = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];

	/*Y座標 - 半径が０よりも小さかったら*/
	if (position.y <= 0.0f)
	{
		position.y = 0.0f;
		this->state->ClearFlag(this->JUMP);
		this->jumpPower = 0.0f;
	}
	this->model->SetPosition(position);

	/*コライダーの更新*/
	this->collider->SetSphere(position, radius);

	/*アニメーションの更新*/
	UpdateAnimation();

	this->model->PlayAnimation(this->nowAnimation,this->animationPlayTime);
}

/// <summary>
/// 描画
/// </summary>
const void Player::Draw()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& camera = Singleton<CameraManager>::GetInstance();

	VECTOR position = this->model->GetPosition();
	VECTOR rotation = this->model->GetRotation();
	printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	printfDx("PLAYER_VELOCITY:%f\n", this->velocity);
	printfDx("%d:IDLE					\n", this->state->CheckFlag(this->IDLE));
	printfDx("%d:REACTION				\n", this->state->CheckFlag(this->REACTION));
	printfDx("%d:DEATH					\n", this->state->CheckFlag(this->DEATH));
	printfDx("%d:LOCK_ON				\n", this->state->CheckFlag(this->LOCK_ON));
	printfDx("%d:STAND					\n", this->state->CheckFlag(this->STAND));
	printfDx("%d:CROUCH					\n", this->state->CheckFlag(this->CROUCH));
	printfDx("%d:RUN					\n", this->state->CheckFlag(this->RUN));
	printfDx("%d:WALK					\n", this->state->CheckFlag(this->WALK));
	printfDx("%d:AVOID					\n", this->state->CheckFlag(this->AVOID));
	printfDx("%d:JUMP					\n", this->state->CheckFlag(this->JUMP));
	printfDx("%d:BLOCK					\n", this->state->CheckFlag(this->BLOCK));
	printfDx("%d:BIG_IMPACT				\n", this->state->CheckFlag(this->BIG_IMPACT));
	printfDx("%d:SMALL_IMPACT			\n", this->state->CheckFlag(this->SMALL_IMPACT));
	printfDx("%d:CASTING				\n", this->state->CheckFlag(this->CASTING));
	printfDx("%d:COMBO_ATTACK			\n", this->state->CheckFlag(this->COMBO_ATTACK));
	printfDx("%d:CROUCH_SLASH			\n", this->state->CheckFlag(this->CROUCH_SLASH));
	printfDx("%d:KICK					\n", this->state->CheckFlag(this->KICK));
	printfDx("%d:PUNCH					\n", this->state->CheckFlag(this->PUNCH));
	printfDx("%d:ROTATION_ATTACK		\n", this->state->CheckFlag(this->ROTATION_ATTACK));
	printfDx("%d:SLASH_1				\n", this->state->CheckFlag(this->SLASH_1));
	printfDx("%d:SLASH_2				\n", this->state->CheckFlag(this->SLASH_2));
	printfDx("%d:ROAR					\n", this->state->CheckFlag(this->ROAR));
	this->model->Draw();
	this->collider->DrawHitSphere();
}

/// <summary>
/// 移動
/// </summary>
void Player::Move()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*回転率の更新*/
	UpdateRotation();

	/*速度の更新*/
	UpdateVelocity();

	/*移動ベクトルの更新*/
	UpdateMoveVector();

	/*向きの更新*/
	//ロックオンしていなかったら
	if (!state->CheckFlag(this->LOCK_ON))
	{
		if (state->CheckFlag(this->MASK_MOVE))
		{
			this->direction = this->moveVector;
		}
		this->direction.y = 0.0f;
	}
	else
	{
		this->direction = VSub(enemy.GetPosition(),this->model->GetPosition());
	}
}
/// <summary>
/// 速度の更新
/// </summary>
void Player::UpdateMoveVector()
{
	/*移動ベクトルを初期化する*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*移動しているときか回避しているときは移動ベクトルを出す*/
	if (this->state->CheckFlag(this->MASK_MOVE) || this->state->CheckFlag(this->AVOID))
	{
		/*回転率をもとに移動ベクトルを出す*/
		this->moveVector = VGet(-sinf(this->moveVectorRotation.y), 0.0f, -cosf(this->moveVectorRotation.y));
		/*移動ベクトルを正規化*/
		this->moveVector = VNorm(this->moveVector);
		/*移動ベクトルに速度を加算*/
		this->moveVector = this->moveVector * this->velocity;
	}
}
/// <summary>
/// 速度の更新
/// </summary>
void Player::UpdateVelocity()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*状態が移動状態でなければ速度を落とす*/
	if (!this->state->CheckFlag(this->MASK_MOVE))
	{
		/*定数型に代入*/
		float DECEL = json.GetJson(JsonManager::FileType::PLAYER)["DECEL"];

		/*もし速度が最大速度以上だったらリターンを返す*/
		if (this->velocity <= 0.0f)
		{
			this->velocity = 0.0f;
			return;
		}

		/*減速させる*/
		this->velocity -= DECEL;
	}
	/*移動していたら速度を最大速度まで上げる*/
	else
	{
		//加速度
		const float ACCEL = json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"];
		//最大速度の決定
		float maxVelocity = 0.0f;
		if (this->state->CheckFlag(this->AVOID))
		{
			maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_VELOCITY"];
		}
		else if (this->state->CheckFlag(this->WALK))
		{
			maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["WALK_VELOCITY"];
		}
		else if (this->state->CheckFlag(this->RUN))
		{
			maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["RUN_VELOCITY"];
		}
		/*もし速度が最大速度以上だったらリターンを返す*/
		if (this->velocity >= maxVelocity)
		{
			this->velocity = maxVelocity;
			return;
		}

		/*加速させる*/
		this->velocity += ACCEL;
	}
}

/// <summary>
/// 座標のgetter
/// </summary>
/// <returns></returns>
const VECTOR Player::GetPosition()const
{
	return this->model->GetPosition();
}

/// <summary>
/// 回転率の更新
/// </summary>
void Player::UpdateRotation()
{
	/*移動できるか*/
	if (!CanRotation())return;

	/*使用する値の準備*/
	const float PI = 180.0f;				//弧度法でのπ
	VECTOR rotation = { 0.0f,0.0f,0.0f };	//回転率
	bool   isInputLStick = false;				//Lスティック入力
	VECTOR cameraDirection = { 0.0f,0.0f,0.0f };	//カメラの向き
	VECTOR playerToBossDirection = { 0.0f,0.0f,0.0f };	//カメラの向き

	/*シングルトンクラスのインスタンスの取得*/
	auto& input  = Singleton<InputManager> ::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager> ::GetInstance();
	auto& json	 = Singleton<JsonManager>  ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	//スティック入力
	this->lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
	//スティックが入力されていたら移動している
	if (lStick.x != 0.0f || lStick.z != 0.0f)
	{
		isInputLStick = true;

		//なにも移動状態が入っていなければWALKをセットする
		if (!this->state->CheckFlag(this->MASK_MOVE))
		{
			this->state->SetFlag(this->WALK);
		}
		//Lスティック押し込みがあれば状態を切り替える
		if (pad & PAD_INPUT_9 && !this->isCount[static_cast<int>(FrameCountType::SWITCH_MOVE_STATE)])
		{
			if (this->state->CheckFlag(this->RUN))
			{
				this->state->ClearFlag(this->RUN);
				this->state->SetFlag(this->WALK);
			}
			else
			{
				this->state->ClearFlag(this->WALK);
				this->state->SetFlag(this->RUN);
			}
			this->isCount[static_cast<int>(FrameCountType::SWITCH_MOVE_STATE)] = true;
		}
	}
	else
	{
		this->state->ClearFlag(this->RUN | this->WALK);
	}
	FrameCount(static_cast<int>(FrameCountType::SWITCH_MOVE_STATE), json.GetJson(JsonManager::FileType::PLAYER)["SWITCH_MOVE_STATE_MAX_FRAME"]);


	/*カメラの向きを出す*/
	//カメラ座標からプレイヤーの座標へのベクトルを出す
	cameraDirection = this->direction = camera.GetCameraDirection();
	//求めたベクトルを正規化する
	cameraDirection = VNorm(cameraDirection);
	//反転していたのでベクトルを反転する
	cameraDirection = cameraDirection * -1.0f;

	/*プレイヤーからボスへの向きを出す*/
	//ボス座標からプレイヤーの座標へのベクトルを出す
	playerToBossDirection = VSub(enemy.GetPosition(), this->model->GetPosition());
	//求めたベクトルを正規化する
	playerToBossDirection = VNorm(playerToBossDirection);


	/*もしロックオンしていたら*/
	if (this->state->CheckFlag(this->LOCK_ON))
	{
		/*カメラの向いている方向をもとにモデルの回転率を出す。*/
		rotation.y = static_cast<float>(
			-atan2(static_cast<double>(playerToBossDirection.z), static_cast<double>(playerToBossDirection.x)));
		rotation.y -= 90.0f * (DX_PI_F / 180.0f);
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(playerToBossDirection.z), static_cast<double>(playerToBossDirection.x))
				- atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
		}
	}
	else
	{
		/*カメラの向いている方向と、プレイヤーが最初に向いていた方向をもとにモデルの回転率を出す。*/
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				- atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
			this->moveVectorRotation = rotation;
		}
	}
	rotation = VGet(rotation.x * PI / DX_PI_F, (rotation.y * PI / DX_PI_F), rotation.z * PI / DX_PI_F);

	if (isInputLStick || this->state->CheckFlag(this->LOCK_ON))
	{
		this->model->SetRotation(rotation);
	}
}

/// <summary>
/// stateが移動状態になっているか
/// </summary>
const bool Player::IsMove()const
{
	return this->state->CheckFlag(this->MASK_MOVE);
}

/// <summary>
/// 攻撃したか
/// </summary>
const bool Player::IsAttack()const
{
	/*攻撃マスクを確認してビットが立っていたらtrueを返す*/
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}

/// <summary>
/// 軽攻撃をしているか
/// </summary>
const bool Player::IsLightAttack()const
{
	return this->state->CheckFlag(this->MASK_ATTACK);
}


/// <summary>
/// 重攻撃をしているか
/// </summary>
const bool Player::IsHeavyAttack()const
{
	return this->state->CheckFlag(this->MASK_ATTACK);
}

/// <summary>
/// 移動ベクトルの補正
/// </summary>
const void Player::FixMoveVector(const VECTOR _fixVector)
{
	this->moveVector = this->moveVector + _fixVector; 
}

/// <summary>
/// コライダーの取得
/// </summary>
const Collider Player::GetCollider()
{
	return *this->collider;
}

/// <summary>
/// アニメーションの更新
/// </summary>
void Player::UpdateAnimation()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*
	HACK:アニメーションの優先順位
	デス→リアクション→攻撃・防御・ジャンプ・回避
	*/

	/*デスしていたら*/
	if (this->state->CheckFlag(this->DEATH))
	{
		this->nowAnimation = static_cast<int>(AnimationType::DEATH);
	}
	/*攻撃していたら*/
	else if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		this->nowAnimation = this->attackAnimationMap[this->state->GetTargetFlag(this->MASK_ATTACK)];
	}
	/*回避していたら*/
	else if (this->state->CheckFlag(this->AVOID))
	{
		this->nowAnimation = static_cast<int>(AnimationType::AVOID);
	}
	/*ジャンプしていたら*/
	else if (this->state->CheckFlag(this->JUMP))
	{
		//移動していたら
		if (this->state->CheckFlag(this->MASK_MOVE))
		{
			//ロックオンしていたら待機状態のジャンプをする
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				this->nowAnimation = static_cast<int>(AnimationType::IDLE_JUMP);
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::MOVE_JUMP);
			}
		}
		else
		{
			this->nowAnimation = static_cast<int>(AnimationType::IDLE_JUMP);
		}
	}
	/*防御していたら*/
	else if (this->state->CheckFlag(this->BLOCK))
	{
		this->nowAnimation = static_cast<int>(AnimationType::BLOCK);
	}
	/*移動していたら*/
	else if (this->state->CheckFlag(this->MASK_MOVE))
	{
		//走り
		if (this->state->CheckFlag(this->RUN))
		{
			//ロックオンしているか
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				//ZとXの入力の絶対値を比べて、大きいほうで判断する
				if (this->lStick.x * this->lStick.x < this->lStick.z * this->lStick.z)
				{
					//後ろ
					if (this->lStick.z > 0)
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_BACK);
					}
					//前
					else
					{
						this->nowAnimation = static_cast<int>(AnimationType::RUN);
					}
				}
				else
				{
					//右
					if (this->lStick.x > 0)
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_RIGHT);
					}
					//左
					else
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_LEFT);
					}
				}
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::RUN);
			}
		}
		//歩き
		if (this->state->CheckFlag(this->WALK))
		{
			//ロックオンしているか
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				//ZとXの入力の絶対値を比べて、大きいほうで判断する
				if (this->lStick.x * this->lStick.x < this->lStick.z * this->lStick.z)
				{
					//後ろ
					if (this->lStick.z > 0)
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_BACK);
					}
					//前
					else
					{
						this->nowAnimation = static_cast<int>(AnimationType::WALK);
					}
				}
				else
				{
					//右
					if (this->lStick.x > 0)
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_RIGHT);
					}
					//左
					else
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_LEFT);
					}
				}
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::WALK);
			}
		}
	}
	/*何もしていなかったら待機状態*/
	else if (this->state->CheckFlag(this->IDLE))
	{
		if (this->state->CheckFlag(this->STAND))
		{
			this->nowAnimation = static_cast<int>(AnimationType::STANDING_IDLE);
		}
		else if (this->state->CheckFlag(this->CROUCH))
		{
			this->nowAnimation = static_cast<int>(AnimationType::CROUCH_IDLE);
		}
	}

	/*アニメーション再生時間を取得*/
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

void Player::StandOrSit()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	/*立ち座り*/
	if (pad & PAD_INPUT_7)
	{
		this->state->SetFlag(this->CROUCH);
	}
	else
	{
		this->state->SetFlag(this->STAND);
	}
}

/// <summary>
/// ロックオン
/// </summary>
void Player::LockOn()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	/*Lスティック押し込みがあればロックオン*/
	if (pad & PAD_INPUT_10 && !this->isCount[static_cast<int>(FrameCountType::SWITCH_LOCK_ON)])
	{
		if (this->state->CheckFlag(this->LOCK_ON))
		{
			this->state->ClearFlag(this->LOCK_ON);
		}
		else
		{
			this->state->SetFlag(this->LOCK_ON);
		}
		this->isCount[static_cast<int>(FrameCountType::SWITCH_LOCK_ON)] = true;
	}
	FrameCount(static_cast<int>(FrameCountType::SWITCH_LOCK_ON), json.GetJson(JsonManager::FileType::PLAYER)["SWITCH_LOCK_ON_MAX_FRAME"]);
}

/// <summary>
/// ジャンプ
/// </summary>
void Player::Jump()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& input = Singleton<InputManager> ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	/*Aボタンが押されたか*/
	if (CanJump() && pad & PAD_INPUT_3)
	{
		this->jumpPower = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"];
		this->state->SetFlag(this->JUMP);
	}

	/*重力を代入*/
	float y = json.GetJson(JsonManager::FileType::PLAYER)["GRAVITY"];

	if (this->state->CheckFlag(this->JUMP))
	{
		float jumpDecel = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_DECEL"];
		this->jumpPower -= jumpDecel;
	}
	this->moveVector.y = y + this->jumpPower;
}
/// <summary>
/// ブロック
/// </summary>
void Player::Block()
{
	/*ブロックできなければ早期リターン*/
	if (!CanBlock())return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	/*LBボタンが押されているか*/
	if (pad & PAD_INPUT_5)
	{
		this->state->SetFlag(this->BLOCK);
		this->state->ClearFlag(this->MASK_MOVE);
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
	}
}

/// <summary>
/// 回避
/// </summary>
void Player::Avoid()
{
	/*回避中にアニメーションが終了していたらフラグを下す*/
	if (this->state->CheckFlag(this->AVOID) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->AVOID);
	}

	/*回避できなければ早期リターン*/
	if (!CanAvoid())return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	/*回避*/
	if (input.GetPadState() & PAD_INPUT_4)
	{
		this->state->SetFlag(this->AVOID);
		this->state->ClearFlag(this->MASK_MOVE);

		//回避用の初速度を入れる
		this->velocity = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_VELOCITY"];
	}
}

/// <summary>
/// 咆哮
/// </summary>
void Player::Roar()
{
	/*咆哮中にアニメーションが終了していたらフラグを下す*/
	if (this->state->CheckFlag(this->ROAR) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->ROAR);
	}

	/*咆哮ができなければ早期リターン*/
	if (!CanRoar())return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	/*咆哮*/
	if (input.GetPadState() & PAD_INPUT_7 && input.GetPadState() & PAD_INPUT_8)
	{
		this->state->SetFlag(this->ROAR);
	}

}
/// <summary>
/// 攻撃
/// </summary>
void Player::Attack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager> ::GetInstance();

	/*攻撃中にアニメーションが終了していたらフラグを下す*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ATTACK);
	}

	/*攻撃できないなら早期リターン*/
	if (!CanAttack())return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	//ガードしていたら
	if (this->state->CheckFlag(this->BLOCK))
	{
		if (pad & PAD_INPUT_2)
		{
			this->state->SetFlag(this->KICK);
		}
	}
	else
	{
		//回転攻撃
		if (pad & PAD_INPUT_6)
		{
			this->state->SetFlag(this->ROTATION_ATTACK);
		}
		//詠唱攻撃
		else if (pad & PAD_INPUT_8)
		{
			this->state->SetFlag(this->CASTING);
		}
		//通常攻撃
		else if (pad & PAD_INPUT_1)
		{
			if (this->attackComboCount >= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_COMBO_NUM"])
			{
				this->attackComboCount = 0;
			}
			this->state->SetFlag(this->attackComboStateMap[this->attackComboCount]);
			this->attackComboCount++;
			this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
			this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = true;
		}
		//Yをしている
		else if (pad & PAD_INPUT_2)
		{
			this->state->SetFlag(this->PUNCH);
		}
	}

	/*もし攻撃していたら移動フラグを下す*/
	if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		this->state->ClearFlag(this->MASK_MOVE);
	}

	/*一定時間攻撃入力がなければコンボを途切れさせる*/
	if (FrameCount(static_cast<int>(FrameCountType::ATTACK_INTERVAL), json.GetJson(JsonManager::FileType::PLAYER)["CONBO_RESET_MAX_FRAME"]))
	{
		this->attackComboCount = 0;
	}
}

bool Player::FrameCount(const int _index, const int _maxFrame)
{
	/*もしカウントが開始していたら*/
	if (this->isCount[_index])
	{
		//カウントを増加させる
		this->frameCount[_index]++;
		//もし最大を越していたらフラグを下してカウントを初期化する
		if (this->frameCount[_index] >= _maxFrame)
		{
			this->isCount[_index] = false;
			this->frameCount[_index] = 0;
			return true;
		}
	}
	return false;
}

const bool Player::CanRotation()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->BLOCK))		return false;//ガード
	if (this->state->CheckFlag(this->CROUCH))		return false;//しゃがみ
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->CROUCH))		return false;//しゃがみ
	return true;
}
const bool Player::CanAvoid()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->BLOCK))		return false;//ガード
	if (this->state->CheckFlag(this->CROUCH))		return false;//しゃがみ
	return true;
}
const bool Player::CanRoar()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->ROAR))			return false;//咆哮
	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->ROAR))			return false;//咆哮
	if (this->state->CheckFlag(this->JUMP))			return false;//ジャンプ
	return true;
}
const bool Player::CanJump()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->ROAR))			return false;//咆哮
	if (this->state->CheckFlag(this->JUMP))			return false;//ジャンプ
	if (this->state->CheckFlag(this->BLOCK))		return false;//ブロック
	return true;
}
const bool Player::DontAnyAction()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->ROAR))			return false;//咆哮
	if (this->state->CheckFlag(this->JUMP))			return false;//ジャンプ
	if (this->state->CheckFlag(this->BLOCK))		return false;//ブロック
	if (this->state->CheckFlag(this->MASK_MOVE))	return false;//移動

	return true;
}