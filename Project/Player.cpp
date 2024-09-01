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
	this->attackAnimationMap.emplace(this->MAIN_ATTACK_1, static_cast<int>(AnimationType::MAIN_1));
	this->attackAnimationMap.emplace(this->MAIN_ATTACK_2, static_cast<int>(AnimationType::MAIN_2));
	this->attackAnimationMap.emplace(this->SPECIAL_ATTACK, static_cast<int>(AnimationType::SPECIAL));
	this->attackTypeMap.emplace(this->MAIN_ATTACK_1, static_cast<int>(AttackType::MAIN_1));
	this->attackTypeMap.emplace(this->MAIN_ATTACK_2, static_cast<int>(AttackType::MAIN_2));
	this->attackTypeMap.emplace(this->SPECIAL_ATTACK, static_cast<int>(AttackType::SPECIAL));
	this->attackComboStateMap.emplace(0, this->MAIN_ATTACK_1);
	this->attackComboStateMap.emplace(1, this->MAIN_ATTACK_2);
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(MV1LoadModel(animationHandle[i].c_str()), animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

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
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //拡大率

	/*変数の初期化*/
	this->direction = VGet(0.0f, 0.0f, -1.0f);
	capsuleColiderData->hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];	 //拡大率

	/*モデルの読み込み*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::PLAYER));

	/*コライダーの初期化*/
	Collidable::Initialize(_physics);
	
	/*物理挙動の初期化*/
	this->rigidbody.Initialize(true);
	this->rigidbody.SetPosition(POSITION);
	this->rigidbody.SetRotation(ROTATION);
	this->rigidbody.SetScale(SCALE);
	this->speed = json.GetJson(JsonManager::FileType::PLAYER)["SPEED"];

	this->state->SetFlag(this->IDLE);
	this->animation->Attach(&this->modelHandle);
	this->attackNumber = 0;
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
	isGround = false;
	for (const auto& object : hitObjects)
	{
		if (object->GetTag() == GameObjectTag::GROUND)
		{
			isGround = true;
			break;
		}
	}

	/*アクション*/
	LockOn();
	Block();
	Attack();
	Move();

	/*もし何もアクションをしていなかったらIdleを入れる*/
	if (DontAnyAction()) { this->state->SetFlag(this->IDLE); }

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
	printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	printfDx("PLAYER_SPEED:%f\n", this->speed);
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

	Avoid();

	/*速度の更新*/
	UpdateSpeed();

	/*移動ベクトルの更新*/
	UpdateMoveVector();

	/*向きの更新*/
	//ロックオンしていなかったら
	if (!state->CheckFlag(this->LOCK_ON))
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
	/*移動ベクトルを初期化する*/
	VECTOR direction = { 0.0f,0.0f,0.0f };

	/*移動しているときか回避しているときは移動ベクトルを出す*/
	if (this->state->CheckFlag(this->MASK_MOVE) || this->state->CheckFlag(this->AVOID))
	{
		/*回転率をもとに移動ベクトルを出す*/
		direction = VGet(-sinf(this->moveVectorRotation.y), 0.0f, -cosf(this->moveVectorRotation.y));
		/*移動ベクトルを正規化*/
		direction = VNorm(direction);

	}
	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);

	this->rigidbody.SetVelocity(newVelocity);
}
/// <summary>
/// 速度の更新
/// </summary>
void Player::UpdateSpeed()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->state->CheckFlag(this->AVOID))
	{
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_VELOCITY"];
	}
	else if (this->state->CheckFlag(this->WALK))
	{
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_VELOCITY"];
	}
	else if (this->state->CheckFlag(this->RUN))
	{
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["RUN_VELOCITY"];
	}
	else
	{
		this->speed = 0.0f;
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

		//LB入力があれば状態を切り替える
		if (pad & PAD_INPUT_5)
		{
			this->state->ClearFlag(this->WALK);
			this->state->SetFlag(this->RUN);
		}
		else
		{
			this->state->ClearFlag(this->RUN);
			this->state->SetFlag(this->WALK);
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
	playerToBossDirection = VSub(enemy.GetPosition(), this->rigidbody.GetPosition());
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

	if (isInputLStick || this->state->CheckFlag(this->LOCK_ON))
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
/// アニメーションの更新
/// </summary>
void Player::UpdateAnimation()
{
	/*死亡アニメーション*/
	if (this->state->CheckFlag(this->DEATH))
	{
		this->nowAnimation = static_cast<int>(AnimationType::DEATH);
	}
	/*リアクション*/
	else if (this->state->CheckFlag(this->REACTION))
	{
		this->nowAnimation = static_cast<int>(AnimationType::SMALL_IMPACT);
	}
	/*攻撃*/
	else if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		this->nowAnimation = this->attackAnimationMap[this->attackType];
	}
	/*ガード*/
	else if (this->state->CheckFlag(this->BLOCK))
	{
		this->nowAnimation = static_cast<int>(AnimationType::BLOCK);
	}
	/*回避*/
	else if (this->state->CheckFlag(this->AVOID))
	{
		this->nowAnimation = static_cast<int>(AnimationType::AVOID);
	}
	/*移動*/
	else if (this->state->CheckFlag(this->MASK_MOVE))
	{
		if (this->state->CheckFlag(this->RUN))
		{
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				if (this->lStick.x < 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_LEFT);
				}
				else if (this->lStick.x > 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_RIGHT);
				}
				else if (this->lStick.z < 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_BACK);
				}
				else
				{
					this->nowAnimation = static_cast<int>(AnimationType::RUN);
				}
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::RUN);
			}
		}
		else
		{
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				if (this->lStick.x < 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_LEFT);
				}
				else if (this->lStick.x > 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_RIGHT);
				}
				else if (this->lStick.z < 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_BACK);
				}
				else
				{
					this->nowAnimation = static_cast<int>(AnimationType::WALK);
				}
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::WALK);
			}
		}
	}
	/*待機*/
	else if (this->state->CheckFlag(this->IDLE))
	{
		this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	}
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
	int pad = input.GetPadState();

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
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();

	if (!CanBlock())return;

	if (pad & PAD_INPUT_7)
	{
		this->state->ClearFlag(this->MASK_MOVE);
		this->state->SetFlag(this->BLOCK);
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = true;
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = false;
	}
}

/// <summary>
/// 回避
/// </summary>
void Player::Avoid()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();

	if (this->state->CheckFlag(this->AVOID) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->AVOID);
	}

	if (FrameCount(static_cast<int>(FrameCountType::AVOID), json.GetJson(JsonManager::FileType::PLAYER)["AVOID_MAX_FRAME"]))
	{
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = false;
	}


	if (!CanAvoid())return;

	if (pad & PAD_INPUT_4)
	{
		this->state->SetFlag(this->AVOID);
		this->isCount[static_cast<int>(FrameCountType::AVOID)] = true;
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = true;
	}

}
/// <summary>
/// 攻撃
/// </summary>
void Player::Attack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();

	int pad = input.GetPadState();

	/*攻撃アニメーションが終了していたらフラグを下す*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ATTACK);
	}

	if (!CanAttack())return;

	if (pad & PAD_INPUT_1)
	{
		this->state->ClearFlag(this->MASK_MOVE);
		this->state->SetFlag(this->attackComboStateMap[this->attackComboCount]);
		this->attackType = this->attackComboStateMap[this->attackComboCount];
		this->attackComboCount++;
		if (this->attackComboCount > json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_COMBO_NUM"])
		{
			this->attackComboCount = 0;
		}
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
		this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = true;
		attack.OnIsStart(this->attackTypeMap[this->attackType]);
		this->attackNumber++;
	}
	else if (pad & PAD_INPUT_2)
	{
		this->state->ClearFlag(this->MASK_MOVE);
		this->state->SetFlag(this->SPECIAL_ATTACK);
		this->attackType = this->SPECIAL_ATTACK;
		this->attackComboCount = 0;
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
		this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = false;
		attack.OnIsStart(this->attackTypeMap[this->attackType]);
		this->attackNumber++;
	}

	/*一定時間Xを押していなかったらコンボを途切れさせる*/
	if (this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)])
	{
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)]++;
		if (this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] >= json.GetJson(JsonManager::FileType::PLAYER)["CONBO_RESET_MAX_FRAME"])
		{
			this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
			this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = false;
			this->attackComboCount = 0;
		}
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
	//if (!this->state->CheckFlag(this->BLOCK | this->AVOID))
	//{
	//	this->hp -= _damage;
	//	this->state->ClearFlag(this->MASK_ALL);
	//	if (_damage >= 20)
	//	{
	//		this->state->SetFlag(this->BIG_IMPACT);
	//	}
	//	else
	//	{
	//		this->state->SetFlag(this->SMALL_IMPACT);
	//	}
	//}
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

//HPの取得
const int Player::GetHP()const
{ 
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	return capsuleColiderData->GetHP();
}
