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
	for (int i = 0; i < this->COLLIDER_NUM; i++)
	{
		this->collider[i] = new Collider();
	}

	/*vectorの追加*/
	for (int i = 0; i < this->COUNT_NUM; i++)
	{
		this->frameCount.emplace_back(0);
		this->isCount.emplace_back(false);
	}

	/*アニメーションの設定*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);
	this->attackAnimationMap.emplace(this->MAIN_ATTACK_1, static_cast<int>(AnimationType::MAIN_1));
	this->attackAnimationMap.emplace(this->MAIN_ATTACK_2, static_cast<int>(AnimationType::MAIN_2));
	this->attackAnimationMap.emplace(this->SPECIAL_ATTACK, static_cast<int>(AnimationType::SPECIAL));
	this->attackComboStateMap.emplace(0, this->MAIN_ATTACK_1);
	this->attackComboStateMap.emplace(1, this->MAIN_ATTACK_2);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	for (int i = 0; i < this->COLLIDER_NUM; i++)
	{
		DeleteMemberInstance(this->collider[i]);
	}
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
	this->hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];	 //拡大率
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
	
	/*リアクション*/
	Reaction();

	/*移動*/
	Move();

	/*ロックオン*/
	LockOn();
	
	/*ガード*/
	Block();

	/*回避*/
	Avoid();

	/*攻撃*/
	Attack();

	/*なにも移動していなければ待機状態をセットする*/
	if (DontAnyAction())
	{
		this->state->SetFlag(this->IDLE);
	}
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	VECTOR position = this->model->GetPosition() + this->moveVector;
	const float  radius = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];

	this->model->SetPosition(position);

	/*コライダーの更新*/
	float hitHeight = json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];
	this->collider[static_cast<int>(ColliderType::CHARACTER)]->SetCapsule(position,hitHeight, radius);

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
	printfDx("%d:RUN					\n", this->state->CheckFlag(this->RUN));
	printfDx("%d:WALK					\n", this->state->CheckFlag(this->WALK));
	printfDx("%d:AVOID					\n", this->state->CheckFlag(this->AVOID));
	printfDx("%d:BLOCK					\n", this->state->CheckFlag(this->BLOCK));
	printfDx("%d:BIG_IMPACT				\n", this->state->CheckFlag(this->BIG_IMPACT));
	printfDx("%d:SMALL_IMPACT			\n", this->state->CheckFlag(this->SMALL_IMPACT));
	printfDx("%d:CASTING				\n", this->state->CheckFlag(this->MAIN_ATTACK_1));
	printfDx("%d:COMBO_ATTACK			\n", this->state->CheckFlag(this->MAIN_ATTACK_2));
	printfDx("%d:KICK					\n", this->state->CheckFlag(this->SPECIAL_ATTACK));
	this->model->Draw();
	this->collider[static_cast<int>(ColliderType::CHARACTER)]->DrawHitCapsule();
	this->collider[static_cast<int>(ColliderType::ATTACK)]->DrawHitSphere();
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
		this->direction = VNorm(this->direction);
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

	if (this->state->CheckFlag(this->AVOID))
	{
		this->velocity = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_VELOCITY"];
	}
	else if (this->state->CheckFlag(this->WALK))
	{
		this->velocity = json.GetJson(JsonManager::FileType::PLAYER)["WALK_VELOCITY"];
	}
	else if (this->state->CheckFlag(this->RUN))
	{
		this->velocity = json.GetJson(JsonManager::FileType::PLAYER)["RUN_VELOCITY"];
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
	cameraDirection = camera.GetCameraDirection();
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
/// ダメージの取得
/// </summary>
const int Player::GetDamage()const
{
	return this->damage;
}

/// <summary>
/// 移動ベクトルの補正
/// </summary>
void Player::FixMoveVector(const VECTOR _fixVector)
{
	this->moveVector = this->moveVector + _fixVector; 
}

/// <summary>
/// コライダーの取得
/// </summary>
const Collider Player::GetCharacterCollider()
{
	return *this->collider[static_cast<int>(ColliderType::CHARACTER)];
}
const Collider Player::GetAttackCollider()
{
	return *this->collider[static_cast<int>(ColliderType::ATTACK)];
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
	else if (this->state->CheckFlag(this->MASK_REACTION))
	{
		if (this->state->CheckFlag(this->BIG_IMPACT))
		{
			this->nowAnimation = static_cast<int>(AnimationType::BIG_IMPACT);
		}
		else if (this->state->CheckFlag(this->SMALL_IMPACT))
		{
			this->nowAnimation = static_cast<int>(AnimationType::SMALL_IMPACT);
		}
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
		this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	}

	/*アニメーション再生時間を取得*/
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}
void Player::Reaction()
{
	if (this->state->CheckFlag(this->MASK_REACTION) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_REACTION);
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

	/*攻撃コライダー用変数*/
	float radius = 0.0f;
	float offsetScale = 0.0f;
	float offsetY = 0.0f;
	VECTOR position = { 0.0f,0.0f,0.0f };
	int attackType = -1;

	/*パッド入力の取得*/
	int pad = input.GetPadState();

	//特殊攻撃
	if (pad & PAD_INPUT_2)
	{
		this->state->SetFlag(this->SPECIAL_ATTACK);
		attackType = static_cast<int>(AttackType::SPECIAL);
	}
	//通常攻撃
	else if (pad & PAD_INPUT_1)
	{
		if (this->attackComboCount >= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_COMBO_NUM"])
		{
			this->attackComboCount = 0;
		}
		switch (this->attackComboCount)
		{
		case 0:
			attackType = static_cast<int>(AttackType::MAIN_1);
			break;
		case 1:
			attackType = static_cast<int>(AttackType::MAIN_2);
			break;
		}

		this->state->SetFlag(this->attackComboStateMap[this->attackComboCount]);
		this->attackComboCount++;
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
		this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = true;
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

	/*コライダーの更新*/
	if (attackType != static_cast<int>(AttackType::NONE))
	{
		radius = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_RADIUS"][attackType];
		offsetScale = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_SCALE"][attackType];
		offsetY = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"][attackType];
		position = this->model->GetPosition() + VScale(this->direction, offsetScale);
		position.y += offsetY;
		this->damage = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_DAMAGE"][attackType];
		this->attackNumber++;
	}
	this->collider[static_cast<int>(ColliderType::ATTACK)]->SetSphere(position, radius);
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
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	return true;
}
const bool Player::CanAvoid()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->BLOCK))		return false;//ガード
	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	return true;
}
const bool Player::DontAnyAction()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//リアクション
	if (this->state->CheckFlag(this->AVOID))		return false;//回避
	if (this->state->CheckFlag(this->BLOCK))		return false;//ブロック
	if (this->state->CheckFlag(this->MASK_MOVE))	return false;//移動

	return true;
}
void Player::CalcDamage(const int _damage) 
{ 
	if (!this->state->CheckFlag(this->BLOCK | this->AVOID))
	{
		this->hp -= _damage;
		this->state->ClearFlag(this->MASK_ALL);
		if (_damage >= 20)
		{
			this->state->SetFlag(this->BIG_IMPACT);
		}
		else
		{
			this->state->SetFlag(this->SMALL_IMPACT);
		}
	}
}