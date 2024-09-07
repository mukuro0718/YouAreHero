#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "Animation.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "Player.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "EnemyManager.h"
#include "PlayerAttackManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: Collidable		(Collidable::Priority::HIGH, GameObjectTag::PLAYER, GoriLib::ColliderData::Kind::CAPSULE, false)
	, modelHandle		(-1)
	, isGround			(false)
	, speed				(0.0f)
	, attackComboCount  (0)
	, prevHitNum		(0)
	, jumpPower			(0.0f)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*インスタンスの作成*/
	this->state = new BitFlag();
	this->animation = new Animation();

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
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(MV1LoadModel(animationHandle[i].c_str()), animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);
	//アニメーションマップの設定
	this->animationMap.emplace(this->IDLE, static_cast<int>(AnimationType::IDLE));
	this->animationMap.emplace(this->ROLL, static_cast<int>(AnimationType::ROLL));
	this->animationMap.emplace(this->DEATH, static_cast<int>(AnimationType::DEATH));
	this->animationMap.emplace(this->BLOCK, static_cast<int>(AnimationType::BLOCK));
	this->animationMap.emplace(this->JUMP, static_cast<int>(AnimationType::JUMP));
	this->animationMap.emplace(this->REACTION, static_cast<int>(AnimationType::REACTION));
	this->animationMap.emplace(this->BLOCK_REACTION, static_cast<int>(AnimationType::BLOCK_REACTION));
	this->animationMap.emplace(this->RUNNING, static_cast<int>(AnimationType::RUNNING));
	this->animationMap.emplace(this->WALK_BACK, static_cast<int>(AnimationType::WALK_BACK));
	this->animationMap.emplace(this->WALK_FRONT, static_cast<int>(AnimationType::WALK_FRONT));
	this->animationMap.emplace(this->WALK_LEFT, static_cast<int>(AnimationType::WALK_LEFT));
	this->animationMap.emplace(this->WALK_RIGHT, static_cast<int>(AnimationType::WALK_RIGHT));
	this->animationMap.emplace(this->SLASH, static_cast<int>(AnimationType::SLASH));


	/*コライダーデータの作成*/
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	capsuleColiderData->radius = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];
	capsuleColiderData->height = json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];
	capsuleColiderData->isCutDamage = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->state);
	this->frameCount.clear();
	this->isCount.clear();
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize(GoriLib::Physics* _physics)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //拡大率

	/*変数の初期化*/
	this->direction				= VGet(0.0f, 0.0f, -1.0f);
	capsuleColiderData->hp		= json.GetJson(JsonManager::FileType::PLAYER)["HP"];	 //HP
	this->stamina				= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];	 //スタミナ

	/*モデルの読み込み*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::PLAYER));

	/*コライダーの初期化*/
	Collidable::Initialize(_physics);
	
	/*物理挙動の初期化*/
	this->rigidbody.Initialize(true);
	this->rigidbody.SetPosition(POSITION);
	this->rigidbody.SetRotation(ROTATION);
	this->rigidbody.SetScale(SCALE);
	this->speed = 0.0f;

	this->state->SetFlag(this->IDLE);
	this->animation->Attach(&this->modelHandle);
	this->attackNumber = 0;
	this->jumpPower = 0.0f;
}

/// <summary>
/// 後処理
/// </summary>
void Player::Finalize(GoriLib::Physics* _physics)
{
	/*物理登録の解除*/
	Collidable::Finalize(_physics);
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(GoriLib::Physics* _physics)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	//地面についているかを判定
	auto capsuleColliderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	VECTOR start = rigidbody.GetPosition();
	VECTOR end = VGet(start.x, start.y + capsuleColliderData->height, start.z);
	auto hitObjects = _physics->IsCollideLine(start, end);
	this->isGround = false;
	for (const auto& object : hitObjects)
	{
		if (object->GetTag() == GameObjectTag::GROUND)
		{
			this->isGround = true;
			break;
		}
	}
	/*フラグの初期化*/
	this->state->ClearFlag(this->MASK_ALL);

	/*アクション*/
	Death();
	Reaction();
	LockOn();
	Attack();
	Move();
	Rolling();
	Block();
	Jump();
	//もし何もアクションをしていなかったらIdleを入れる
	if (DontAnyAction()) 
	{ 
		this->state->SetFlag(this->IDLE); 
	}

	//状態が歩きまたは待機の時のみスタミナを回復する（その他スタミナ消費はその場所で行っている
	//スタミナ計算の場所が散らばっているので統一したい
	if (!this->state->CheckFlag(this->MASK_CANT_RECOVERY_STAMINA))
	{
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);
	}

	/*アニメーションの更新*/
	UpdateAnimation();
	float animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	VECTOR position = this->rigidbody.GetPosition();
	this->animation->Play(&this->modelHandle, position, this->nowAnimation, animationPlayTime);
}

/// <summary>
/// 描画
/// </summary>
const void Player::Draw()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& camera = Singleton<CameraManager>::GetInstance();

	VECTOR position = this->rigidbody.GetPosition();
	VECTOR rotation = this->rigidbody.GetRotation();
#if _DEBUG
	printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f	\n", position.x, position.y, position.z);
	printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f	\n", rotation.x, rotation.y, rotation.z);
	printfDx("PLAYER_SPEED:%f					\n", this->speed);
	printfDx("%d:IDLE							\n", this->state->CheckFlag(this->IDLE));
	printfDx("%d:LOCK_ON						\n", this->isLockOn);
	printfDx("%d:ROLL							\n", this->state->CheckFlag(this->ROLL));
	printfDx("%d:DEATH						\n", this->state->CheckFlag(this->DEATH));
	printfDx("%d:BLOCK						\n", this->state->CheckFlag(this->BLOCK));
	printfDx("%d:JUMP							\n", this->state->CheckFlag(this->JUMP));
	printfDx("%d:REACTION						\n", this->state->CheckFlag(this->REACTION));
	printfDx("%d:BLOCK_REACTION				\n", this->state->CheckFlag(this->BLOCK_REACTION));
	printfDx("%d:RUNNING						\n", this->state->CheckFlag(this->RUNNING));
	printfDx("%d:WALK_BACK					\n", this->state->CheckFlag(this->WALK_BACK));
	printfDx("%d:WALK_FRONT					\n", this->state->CheckFlag(this->WALK_FRONT));
	printfDx("%d:WALK_LEFT					\n", this->state->CheckFlag(this->WALK_LEFT));
	printfDx("%d:WALK_RIGHT					\n", this->state->CheckFlag(this->WALK_RIGHT));
	printfDx("%d:SLASH						\n", this->state->CheckFlag(this->SLASH));
	printfDx("%d:HIT_NUM						\n", this->GetHitNumber());
#endif // _DEBUG

	MV1SetPosition(this->modelHandle, this->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->rigidbody.GetRotation());
	MV1SetScale(this->modelHandle, this->rigidbody.GetScale());
	MV1DrawModel(this->modelHandle);
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
	UpdateSpeed();

	/*移動ベクトルの更新*/
	UpdateMoveVector();

	/*向きの更新*/
	//ロックオンしていなかったら
	if (!this->isLockOn)
	{
		if (state->CheckFlag(this->MASK_MOVE))
		{
			this->direction = this->rigidbody.GetVelocity();
			this->direction = VNorm(this->direction);
		}
		this->direction.y = 0.0f;
	}
	else
	{
		this->direction = VSub(enemy.GetPosition(),this->rigidbody.GetPosition());
		this->direction = VNorm(this->direction);
	}
}
/// <summary>
/// 速度の更新
/// </summary>
void Player::UpdateMoveVector()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*移動ベクトルを初期化する*/
	VECTOR direction = { 0.0f,0.0f,0.0f };

	/*移動しているときか回避しているときは移動ベクトルを出す*/
	if (this->state->CheckFlag(this->MASK_CAN_VELOCITY))
	{
		/*回転率をもとに移動ベクトルを出す*/
		direction = VGet(-sinf(this->moveVectorRotation.y), 0.0f, -cosf(this->moveVectorRotation.y));
		/*移動ベクトルを正規化*/
		direction = VNorm(direction);
	}
	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = rigidbody.GetVelocity();
	float yVelocity = prevVelocity.y - this->jumpPower;

	/*ジャンプ力の計算*/
	if (this->state->CheckFlag(this->JUMP))
	{
		const float MIN_POWER = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"] * -1.0;
		this->jumpPower += static_cast<float>(json.GetJson(JsonManager::FileType::PLAYER)["JUMP_DECEL"]);
		if (this->jumpPower < MIN_POWER)
		{
			this->jumpPower = 0.0f;
		}
	}

	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y + this->jumpPower, aimVelocity.z);

	this->rigidbody.SetVelocity(newVelocity);
}
/// <summary>
/// 速度の更新
/// </summary>
void Player::UpdateSpeed()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	float maxSpeed = 0.0f;
	
	/*移動していたら*/
	if (this->state->CheckFlag(this->MASK_MOVE))
	{
		//走り
		if (this->state->CheckFlag(this->RUNNING))
		{
			maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"];
		}
		//歩き
		else
		{
			maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
		}
	}

	/*速度の設定*/
	//最大速度が０じゃなければ最大速度まで加速する
	if (maxSpeed != 0)
	{
		this->speed += static_cast<float>(json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"]);
		//最大速度を超えないように調整する
		if (this->speed >= maxSpeed)
		{
			this->speed = maxSpeed;
		}
	}
	else
	{
		this->speed += static_cast<float>(json.GetJson(JsonManager::FileType::PLAYER)["DECEL"]);
		//最大速度を超えないように調整する
		if (this->speed <= 0)
		{
			this->speed = 0;
		}
	}
}

/// <summary>
/// 座標のgetter
/// </summary>
/// <returns></returns>
const VECTOR Player::GetPosition()const
{
	return this->rigidbody.GetPosition();
}

/// <summary>
/// 回転率の更新
/// </summary>
void Player::UpdateRotation()
{
	/*移動できるか*/
	if (!CanRotation())return;

	/*初期化*/
	const float PI				  		= 180.0f;						//弧度法でのπ
	VECTOR		rotation				= VGet(0.0f,0.0f,0.0f );	//回転率
	bool		isInputLStick			= false;						//Lスティック入力
	VECTOR		cameraDirection			= VGet(0.0f,0.0f,0.0f );	//カメラの向き
	VECTOR		playerToTargetDirection = VGet(0.0f,0.0f,0.0f );	//カメラの向き
	VECTOR		wasd					= VGet(0.0f, 0.0f, 0.0f);	//wasd入力
	VECTOR		lStick					= VGet(0.0f, 0.0f, 0.0f);	//lStick入力(上:Z+ 下:Z- 左:x- 右:x+)
	this->state->ClearFlag(this->MASK_MOVE);

	/*シングルトンクラスのインスタンスの取得*/
	auto& input  = Singleton<InputManager> ::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager> ::GetInstance();
	auto& json	 = Singleton<JsonManager>  ::GetInstance();

	/*パッド入力の取得*/
	int pad = input.GetPadState();
	//スティック入力
	lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));

	/*移動状態の切り替え*/
	//スティック入力があるか
	if (lStick.x != 0.0f || lStick.z != 0.0f)
	{
		isInputLStick = true;
		//ロックオン状態か
		if (this->isLockOn)
		{
			//左
			if (lStick.x < 0)
			{
				this->state->SetFlag(this->WALK_LEFT);
			}
			//右
			else if (lStick.x > 0)
			{
				this->state->SetFlag(this->WALK_RIGHT);
			}
			//前
			else if (lStick.z < 0)
			{
				this->state->SetFlag(this->WALK_FRONT);
			}
			//後ろ
			else
			{
				this->state->SetFlag(this->WALK_BACK);
			}
		}
		//前に走る
		else
		{
			this->state->SetFlag(this->RUNNING);
		}
	}


	/*カメラの向きを出す*/
	//カメラ座標からプレイヤーの座標へのベクトルを出す
	cameraDirection = camera.GetCameraDirection();
	//求めたベクトルを正規化する
	cameraDirection = VNorm(cameraDirection);
	//反転していたのでベクトルを反転する
	cameraDirection = cameraDirection * -1.0f;

	/*プレイヤーからボスへの向きを出す*/
	//ボス座標からプレイヤーの座標へのベクトルを出す
	playerToTargetDirection = VSub(enemy.GetPosition(), this->rigidbody.GetPosition());
	//求めたベクトルを正規化する
	playerToTargetDirection = VNorm(playerToTargetDirection);


	/*もしロックオンしていたら*/
	if (this->isLockOn)
	{
		/*カメラの向いている方向をもとにモデルの回転率を出す。*/
		rotation.y = static_cast<float>(
			-atan2(static_cast<double>(playerToTargetDirection.z), static_cast<double>(playerToTargetDirection.x)));
		rotation.y -= 90.0f * (DX_PI_F / 180.0f);
		if (isInputLStick)
		{
			lStick = VNorm(lStick);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(playerToTargetDirection.z), static_cast<double>(playerToTargetDirection.x))
				- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
		}
	}
	else
	{
		/*カメラの向いている方向と、プレイヤーが最初に向いていた方向をもとにモデルの回転率を出す。*/
		if (isInputLStick)
		{
			lStick = VNorm(lStick);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
			this->moveVectorRotation = rotation;
		}
	}

	if (isInputLStick || this->isLockOn)
	{
		this->rigidbody.SetRotation(rotation);
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
const bool Player::GetIsSlash()const
{
	return this->state->CheckFlag(this->SLASH);
}
/// <summary>
/// ダメージの取得
/// </summary>
const int Player::GetDamage()const
{
	return this->damage;
}


/// <summary>
/// アニメーションの更新
/// </summary>
void Player::UpdateAnimation()
{
	auto& json = Singleton<JsonManager>  ::GetInstance();

	this->nowAnimation = this->animationMap[this->state->GetFlag()];
}
void Player::Reaction()
{
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	if (this->prevHitNum != capsuleColiderData->GetHitNumber())
	{

	}
}

/// <summary>
/// ロックオン
/// </summary>
void Player::LockOn()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	//pad入力
	int pad = input.GetPadState();

	//LTが押されたか
	if (pad & PAD_INPUT_6)
	{
		this->isLockOn = true;
	}
	else
	{
		this->isLockOn = false;
	}
}

/// <summary>
/// デス
/// </summary>
void Player::Death()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);

	/*もしHPが０未満だったら*/
	if (capsuleColiderData->GetHP() < 0)
	{
		this->state->SetFlag(this->DEATH);
	}
}
/// <summary>
/// ブロック
/// </summary>
void Player::Block()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);

	/*ブロックできるか*/
	if (!CanBlock())return;

	/*LTが押されたか*/
	if (pad & PAD_INPUT_7)
	{
		this->state->SetFlag(this->BLOCK);
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
	}
}
/// <summary>
/// ジャンプ
/// </summary>
void Player::Jump()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();

	if (this->state->CheckFlag(this->JUMP) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->JUMP);
	}


	/*今回避できるか*/
	if (!CanJump())return;

	/*消費スタミナは足りるのか*/
	if (!CanAction(json.GetJson(JsonManager::FileType::PLAYER)["JUMP_STAMINA_CONSUMPTION"]))return;

	if ((pad & PAD_INPUT_3))
	{
		this->state->SetFlag(this->JUMP);
		this->state->ClearFlag(this->MASK_MOVE | this->IDLE);
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["JUMP_STAMINA_CONSUMPTION"]);
		this->jumpPower = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"];
	}
}
/// <summary>
/// 回避
/// </summary>
void Player::Rolling()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();

	if (this->state->CheckFlag(this->ROLL) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->ROLL);
	}

	if (FrameCount(static_cast<int>(FrameCountType::AVOID), json.GetJson(JsonManager::FileType::PLAYER)["AVOID_MAX_FRAME"]))
	{
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = false;
	}

	/*今回避できるか*/
	if (!CanRolling())return;

	/*消費スタミナは足りるのか*/
	if (!CanAction(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]))return;

	if ((pad & PAD_INPUT_4) && !this->isCount[static_cast<int>(FrameCountType::AVOID)])
	{
		this->state->SetFlag(this->ROLL);
		this->isCount[static_cast<int>(FrameCountType::AVOID)] = true;
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = true;
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]);
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["ROLLING_SPEED"];
	}

}
/// <summary>
/// 攻撃
/// </summary>
void Player::Attack()
{
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ATTACK);
	}

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();

	/*pad入力*/
	int pad = input.GetPadState();

	/*攻撃できるか*/
	if (!CanAttack())return;

	/*Xが押されたか*/
	if (pad & PAD_INPUT_1)
	{
		this->state->SetFlag(this->SLASH);
		this->attackNumber++;
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
	if (this->state->CheckFlag(this->REACTION))		return false;//リアクション
	if (this->state->CheckFlag(this->DEATH)	)		return false;//デス
	if (this->state->CheckFlag(this->ROLL))			return false;//回避
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//回避
	if (this->state->CheckFlag(this->JUMP))			return false;//ジャンプ
	return true;
}
const bool Player::CanRolling()const
{
	if (this->isLockOn)									  return false;//ロックオン
	if (this->state->CheckFlag(this->MASK_ATTACK))	  return false;//攻撃
	if (this->state->CheckFlag(this->BLOCK))		  return false;//ブロック
	if (this->state->CheckFlag(this->MASK_REACTION)) return false;//リアクション
	if (this->state->CheckFlag(this->JUMP))			  return false;//ジャンプ
	if (this->state->CheckFlag(this->DEATH))		  return false;//死亡
	if (this->state->CheckFlag(this->ROLL))			  return false;//回避
	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->REACTION))	return false;//リアクション
	if (this->state->CheckFlag(this->DEATH))	return false;//デス
	if (this->state->CheckFlag(this->ROLL))		return false;//回避
	if (this->state->CheckFlag(this->SLASH))	return false;//回避
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->REACTION))	return false;//リアクション
	if (this->state->CheckFlag(this->DEATH))	return false;//デス
	if (this->state->CheckFlag(this->ROLL))		return false;//回避
	if (this->state->CheckFlag(this->SLASH))	return false;//攻撃
	if (this->state->CheckFlag(this->JUMP))		return false;//ジャンプ
	return true;
}
const bool Player::CanJump()const
{
	if (this->state->CheckFlag(this->REACTION))	return false;//リアクション
	if (this->state->CheckFlag(this->DEATH))	return false;//デス
	if (this->state->CheckFlag(this->ROLL))		return false;//回避
	if (this->state->CheckFlag(this->SLASH))	return false;//攻撃
	if (this->state->CheckFlag(this->JUMP))		return false;//ジャンプ
	return true;
}
const bool Player::DontAnyAction()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//攻撃
	if (this->state->CheckFlag(this->REACTION))		return false;//リアクション
	if (this->state->CheckFlag(this->ROLL))			return false;//回避
	if (this->state->CheckFlag(this->DEATH))		return false;//ブロック
	if (this->state->CheckFlag(this->MASK_MOVE))	return false;//移動
	if (this->state->CheckFlag(this->JUMP))			return false;//ジャンプ
	return true;
}

void Player::OnCollide(const Collidable& _colider)
{
	std::string message = "プレイヤーが";
	if (_colider.GetTag() == GameObjectTag::BOSS)
	{
		message += "ボス";
	}
	else if (_colider.GetTag() == GameObjectTag::GROUND)
	{
		message += "地面";
	}

	message += "と当たった\n";
	printfDx(message.c_str());
}

/// <summary>
/// HPの取得
/// </summary>
const int Player::GetHP()const
{ 
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	return capsuleColiderData->GetHP();
}
/// <summary>
/// スタミナの取得
/// </summary>
const float Player::GetStamina()const
{
	return this->stamina;
}
/// <summary>
/// 必要な消費スタミナと現在のスタミナの残量を比べてアクションが可能ならtrueを返す
/// </summary>
/// <returns></returns>
const bool Player::CanAction(const float _staminaConsumed)const
{
	float staminaConsumed = _staminaConsumed * -1.0f;
	/*スタミナの消費量が現在のスタミナの総量よりも多ければfalseを返す*/
	if (staminaConsumed > this->stamina)return false;
	return true;
}

/// <summary>
/// スタミナの回復処理
/// </summary>
void Player::CalcStamina(const float _staminaConsumed)
{
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*それ以外の状態だったら状態に応じてスタミナを消費する*/
	//走り
	this->stamina += _staminaConsumed;
	/*上限値、下限値を超えないように調整*/
	if (this->stamina >= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"])
	{
		this->stamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	}
	else if (this->stamina < 0)
	{
		this->stamina = 0;
	}
}
