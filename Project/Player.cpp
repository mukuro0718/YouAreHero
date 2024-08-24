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
	this->frameCount.emplace_back(0);
	this->isCount.emplace_back(false);

	/*アニメーションの設定*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::STANDING_IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);
	this->attackAnimationMap.emplace(this->CASTING				, static_cast<int>(AnimationType::CASTING));
	this->attackAnimationMap.emplace(this->COMBO_ATTACK			, static_cast<int>(AnimationType::COMBO));
	this->attackAnimationMap.emplace(this->CROUCH_SLASH			, static_cast<int>(AnimationType::CROUCH_SLASH));
	this->attackAnimationMap.emplace(this->JUMP_ATTACK			, static_cast<int>(AnimationType::JUMP_ATTACK));
	this->attackAnimationMap.emplace(this->JUMP_ROTATION_ATTACK	, static_cast<int>(AnimationType::JUMP_ROTATION_ATTACK));
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

	/*回転率の更新*/
	UpdateRotation();

	/*移動*/
	Move();

	/*ジャンプ*/
	Jump();
	
	/*ガード*/

	/*攻撃*/
	Attack();

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
	if (position.y - radius <= 0)
	{
		position.y = radius;
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
	this->model->Draw();
	this->collider->DrawHitSphere();
}

/// <summary>
/// 移動
/// </summary>
void Player::Move()
{
	/*移動ベクトルの初期化*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*移動できるか*/

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();

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
		this->direction = camera.GetCameraDirection() * -1.0f;
	}
}
/// <summary>
/// 速度の更新
/// </summary>
void Player::UpdateMoveVector()
{
	if (this->state->CheckFlag(this->MASK_MOVE))
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
		float DECEL = json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"];

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
		/*定数型に代入*/
		float maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["VELOCITY"];
		const float ACCEL = json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"];

		if (this->state->CheckFlag(this->RUN))
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
	/*使用する値の準備*/
	const float PI			= 180.0f;				//弧度法でのπ
	VECTOR rotation			= { 0.0f,0.0f,0.0f };	//回転率
	bool   isInputKey		= false;				//キー入力の取得
	bool   isInputLStick	= false;				//Lスティック入力
	VECTOR cameraDirection	= { 0.0f,0.0f,0.0f };	//カメラの向き
	this->lStick			= { 0.0f,0.0f,0.0f };	//左スティック入力保持変数
	this->wasd				= { 0.0f,0.0f,0.0f };	//wasd入力保持変数
	
	/*シングルトンクラスのインスタンスの取得*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager>::GetInstance();
	auto& input  = Singleton<InputManager> ::GetInstance();
	auto& json   = Singleton<JsonManager>  ::GetInstance();


	/*左スティック入力を代入*/
	lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
	if (lStick.x != 0.0f || lStick.z != 0.0f)
	{
		isInputLStick = true;
	}

	/*wasd入力を代入*/
	if (CheckHitKey(KEY_INPUT_W))
	{
		isInputKey = true;
		wasd.z = -1.0f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		isInputKey = true;
		wasd.z = 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		isInputKey = true;
		wasd.x = -1.0f;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		isInputKey = true;
		wasd.x = 1.0f;
	}

	/*カメラの向きを出す*/
	//カメラ座標からプレイヤーの座標へのベクトルを出す
	cameraDirection = this->direction = camera.GetCameraDirection();
	//求めたベクトルを正規化する
	cameraDirection = VNorm(cameraDirection);
	//反転していたのでベクトルを反転する
	cameraDirection = cameraDirection * -1.0f;

	/*もしロックオンしていたら*/
	if (this->state->CheckFlag(this->LOCK_ON))
	{
		/*カメラの向いている方向をもとにモデルの回転率を出す。*/
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x)));
			rotation.y -= 90.0f * (DX_PI_F / 180.0f);
		if (isInputKey)
		{
			this->wasd = VNorm(this->wasd);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->wasd.z), static_cast<double>(this->wasd.x)));
		}
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
		}
	}
	else
	{
		/*カメラの向いている方向と、プレイヤーが最初に向いていた方向をもとにモデルの回転率を出す。*/
		if (isInputKey)
		{
			this->wasd = VNorm(this->wasd);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->wasd.z), static_cast<double>(this->wasd.x)));
			this->moveVectorRotation = rotation;
		}
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
			this->moveVectorRotation = rotation;
		}
	}
	rotation = VGet(rotation.x * PI / DX_PI_F, (rotation.y * PI / DX_PI_F), rotation.z * PI / DX_PI_F);

	if (isInputKey || isInputLStick || this->state->CheckFlag(this->LOCK_ON))
	{
		this->model->SetRotation(rotation);
	}
}

/// <summary>
/// 攻撃
/// </summary>
void Player::Attack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
}


/// <summary>
/// 空を飛ぶ
/// </summary>
void Player::Jump()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*空を飛べるか*/
	if (!CanJump())return;

	/*変数の準備*/
	float y = json.GetJson(JsonManager::FileType::PLAYER)["GRAVITY"];

	/*BボタンまたはキーボードのBが押されたら*/
	if (this->state->CheckFlag(this->JUMP))
	{
		float jumpDecel = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_DECEL"];
		this->jumpPower -= jumpDecel;
	}
	this->moveVector.y = y + this->jumpPower;
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
	else
	{
		/*回避していたら*/
		if (this->state->CheckFlag(this->AVOID))
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
						if (this->lStick.z < 0)
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
						if (this->lStick.z < 0)
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
	}
	/*何もしていなかったら待機状態*/
	this->nowAnimation = static_cast<int>(AnimationType::STANDING_IDLE);
}

/// <summary>
/// ブロック
/// </summary>
void Player::Block()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager>::GetInstance();

}

void Player::StateChanger()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*変数の準備*/
	int pad = input.GetPadState();

	/*任意の状態を初期化*/
	this->state->ClearFlag(this->MASK_ALWAYS_INITIALIZE);

	/*立ち座り*/
	if (pad & PAD_INPUT_7)
	{
		this->state->SetFlag(this->CROUCH);
	}
	else
	{
		this->state->SetFlag(this->STAND);
	}

	/*移動*/
	if (this->state->CheckFlag(this->MASK_CANT_MOVE))
	{
		//スティック入力
		this->lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
		//スティックが入力されていたら移動している
		if (lStick.x != 0.0f || lStick.z != 0.0f)
		{
			//Lスティック押し込みがあればダッシュ
			if (pad & PAD_INPUT_9)
			{
				this->state->SetFlag(this->RUN);
			}
			else
			{
				this->state->SetFlag(this->WALK);
			}
		}
	}
	/*Lスティック押し込みがあればロックオン*/
	if (pad & PAD_INPUT_10)
	{
		this->state->SetFlag(this->LOCK_ON);
	}

	/*ジャンプ*/
	if (this->state->CheckFlag(this->JUMP) && pad & PAD_INPUT_3)
	{
		this->jumpPower = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"];
		this->state->SetFlag(this->JUMP);
	}

	/*ガード*/
	if (input.GetPadState() & PAD_INPUT_5)
	{
		this->state->SetFlag(this->BLOCK);
	}

	/*回避*/
	if (this->state->CheckFlag(this->MASK_CANT_AVOID) && input.GetPadState() & PAD_INPUT_4)
	{
		this->state->SetFlag(this->AVOID);
	}

	/*咆哮*/
	if (this->state->CheckFlag(this->MASK_CANT_ROAR) && input.GetPadState() & PAD_INPUT_7 && input.GetPadState() & PAD_INPUT_7)
	{
		this->state->SetFlag(this->AVOID);
	}

	/*攻撃*/
	//メイン攻撃
	if (this->state->CheckFlag(this->MASK_CANT_MAIN_ATTACK))
	{
		//ジャンプしていたら
		if (this->state->CheckFlag(this->JUMP))
		{
			//ジャンプ攻撃
			if (input.GetPadState() & PAD_INPUT_1)
			{
				this->state->SetFlag(this->JUMP_ATTACK);
			}
			//ジャンプ回転攻撃
			else if (input.GetPadState() & PAD_INPUT_6)
			{
				this->state->SetFlag(this->JUMP_ROTATION_ATTACK);
			}
		}
		else
		{
			//回転攻撃
			if (input.GetPadState() & PAD_INPUT_6)
			{
				this->state->SetFlag(this->JUMP_ATTACK);
			}
			//詠唱攻撃
			else if (input.GetPadState() & PAD_INPUT_8)
			{
				this->state->SetFlag(this->JUMP_ATTACK);
			}
			//通常攻撃
			else if (input.GetPadState() & PAD_INPUT_1)
			{
				this->state->SetFlag(this->attackComboStateMap[this->attackComboCount]);
				this->attackComboCount++;
				if (this->attackComboCount >= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_COMBO_NUM"])
				{
					this->attackComboCount = 0;
				}
			}
		}
	}
	//サブ攻撃
	if (this->state->CheckFlag(this->MASK_CANT_SUB_ATTACK))
	{
		//Yをしている
		if (this->state->CheckFlag(this->BLOCK))
		{
			//ガードしていたら
			if (this->state->CheckFlag(this->BLOCK))
			{
				this->state->SetFlag(this->KICK);
			}
			else
			{
				this->state->SetFlag(this->PUNCH);
			}
		}
	}
}