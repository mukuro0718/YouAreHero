#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include <iostream>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "PlayerData.h"
#include "CharacterColliderData.h"
#include "Character.h"
#include "Animation.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "Player.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "EnemyManager.h"
#include "PlayerAttackManager.h"
#include "Debug.h"
#include "EffectManager.h"
#include "HitStop.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
	: attackComboCount	(0)
	, healOrbNum		(0)
	, nowAnimation		(static_cast<int>(AnimationType::IDLE))
	, animationPlayTime	(0.0f)
	, moveVectorRotation(Gori::ORIGIN)
	, hitStop			(nullptr)
	, isDecSpeed (false)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->hitStop = new HitStop();
	/*vectorの追加*/
	for (int i = 0; i < this->COUNT_NUM; i++)
	{
		this->frameCount.emplace_back(0);
		this->isCount.emplace_back(false);
	}

	/*アニメーションの設定*/
	vector<int> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);
	//アニメーションマップの設定
	this->animationMap.emplace(this->IDLE			, static_cast<int>(AnimationType::IDLE				));
	this->animationMap.emplace(this->AVOID			, static_cast<int>(AnimationType::AVOID				));
	this->animationMap.emplace(this->DEATH			, static_cast<int>(AnimationType::DEATH				));
	this->animationMap.emplace(this->BLOCK			, static_cast<int>(AnimationType::BLOCK				));
	this->animationMap.emplace(this->REACTION		, static_cast<int>(AnimationType::REACTION			));
	this->animationMap.emplace(this->BLOCK_REACTION	, static_cast<int>(AnimationType::BLOCK_REACTION	));
	this->animationMap.emplace(this->WALK_FRONT		, static_cast<int>(AnimationType::WALK_FRONT		));
	this->animationMap.emplace(this->WALK_BACK		, static_cast<int>(AnimationType::WALK_BACK			));
	this->animationMap.emplace(this->WALK_LEFT		, static_cast<int>(AnimationType::WALK_LEFT			));
	this->animationMap.emplace(this->WALK_RIGHT		, static_cast<int>(AnimationType::WALK_RIGHT		));
	this->animationMap.emplace(this->RUN_FRONT		, static_cast<int>(AnimationType::RUN_FRONT			));
	this->animationMap.emplace(this->RUN_BACK		, static_cast<int>(AnimationType::RUN_BACK			));
	this->animationMap.emplace(this->RUN_LEFT		, static_cast<int>(AnimationType::RUN_LEFT			));
	this->animationMap.emplace(this->RUN_RIGHT		, static_cast<int>(AnimationType::RUN_RIGHT			));
	this->animationMap.emplace(this->COMBO_1		, static_cast<int>(AnimationType::COMBO_1			));
	this->animationMap.emplace(this->COMBO_2		, static_cast<int>(AnimationType::COMBO_2			));
	this->animationMap.emplace(this->COMBO_3		, static_cast<int>(AnimationType::COMBO_3			));
	this->animationMap.emplace(this->SKILL			, static_cast<int>(AnimationType::SKILL				));
	this->animationMap.emplace(this->HEAL			, static_cast<int>(AnimationType::HEAL				));
	this->animationMap.emplace(this->DOWN			, static_cast<int>(AnimationType::DOWN_REACTION		));
	this->animationMap.emplace(this->DOWN_UP		, static_cast<int>(AnimationType::DOWN_UP			));

	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::NORMAL)	 , this->REACTION);
	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::BLOW_BIG)	 , this->DOWN);
	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::BLOW_SMALL) , this->REACTION);
	
	this->attackMap.emplace(static_cast<int>(AttackType::COMBO_1) , this->COMBO_1);
	this->attackMap.emplace(static_cast<int>(AttackType::COMBO_2) , this->COMBO_2);
	this->attackMap.emplace(static_cast<int>(AttackType::COMBO_3) , this->COMBO_3);
	this->attackMap.emplace(static_cast<int>(AttackType::SKILL) , this->SKILL);


	this->whenRunMoveState.emplace(this->WALK_FRONT, this->RUN_FRONT);
	this->whenRunMoveState.emplace(this->WALK_BACK, this->RUN_BACK);
	this->whenRunMoveState.emplace(this->WALK_LEFT, this->RUN_LEFT);
	this->whenRunMoveState.emplace(this->WALK_RIGHT, this->RUN_RIGHT);

	/*コライダーデータの作成*/
	CharacterData* data = new PlayerData;
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::PLAYER, data);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	this->frameCount.clear();
	this->isCount.clear();
	this->animationMap.clear();
	this->reactionMap.clear();
	this->whenRunMoveState.clear();
	DeleteMemberInstance(this->hitStop);
}

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json		= Singleton<JsonManager>::GetInstance();
	auto& asset		= Singleton<LoadingAsset>::GetInstance();
	auto& collider	= dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data		= dynamic_cast<PlayerData&>(*collider.data);

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//座標
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //拡大率
		  VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//回転率
		  rotation.y	  = rotation.y * (DX_PI_F / 180.0f);
	/*変数の初期化*/
	this->isAlive			 = true;
	this->isDraw			 = true;
	this->isGround			 = false;
	this->isInitialize		 = true;
	this->isBlockingMove	 = false;
	this->isStopAnimation	 = false;
	this->speed				 = 0.0f;
	this->entryInterval		 = 0;
	this->moveVectorRotation = Gori::ORIGIN;
	this->nextRotation = Gori::ORIGIN;
	this->nowAnimation		 = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime	 = 0.0f;
	this->attackComboCount	 = static_cast<int>(AttackType::NONE);
	this->healOrbNum		 = json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];	//最大回復オーブ数
	for (int i = 0; i < this->frameCount.size(); i++)
	{
		this->frameCount[i] = 0;
		this->isCount[i] = false;
	}
	
	/*モデルの読み込み*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::PLAYER));

	/*コライダーの初期化*/
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	float height		= json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];		//カプセルの高さ
	collider.topPositon = /*VAdd(collider.rigidbody.GetPosition(), */VGet(0.0f, height, 0.0f)/*)*/;	//カプセルの上座標
	collider.radius		= json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];			//カプセルの半径
	data.hp				= json.GetJson(JsonManager::FileType::PLAYER)["HP"];				//HP
	data.stamina		= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];			//スタミナ
	data.isInvinvible	= false;															//ダメージをカットするか
	data.isGuard		= false;															//ダメージをカットするか
	data.isHit			= false;															//攻撃がヒットしたか
	this->healOrbNum	= json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];	//最大回復オーブ数


	/*状態の初期化*/
	this->state->ClearFlag(this->DEATH);
	this->state->ClearFlag(this->MASK_ALL);
	this->state->SetFlag(this->IDLE);

	/*アニメーションのアタッチ*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// 後処理
/// </summary>
void Player::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void Player::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->hitStop->IsHitStop()) return;

	/*フラグの初期化*/
	this->state->ClearFlag(this->MASK_ALWAYS_TURN_OFF);

	/*アクション*/
	Death();//デス処理
	if (!this->state->CheckFlag(this->DEATH))
	{
		LockOn	();//ロックオン
		Reaction();//リアクション処理
		Attack	();//攻撃処理
		Move	();//移動処理
		Rolling	();//回避処理
		Block	();//防御処理
		Heal	();//回復処理

		//もし何もアクションをしていなかったらIdleを入れる
		if (DontAnyAction())
		{
			this->state->SetFlag(this->IDLE);
		}
		else
		{
			this->state->ClearFlag(this->IDLE);
		}

		//状態が歩きまたは待機の時のみスタミナを回復する（その他スタミナ消費はその場所で行っている
		//スタミナ計算の場所が散らばっているので統一したい
		if (!this->state->CheckFlag(this->MASK_CANT_RECOVERY_STAMINA))
		{
			CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);
		}
	}
	else
	{
		this->speed = 0.0f;
		VECTOR direction = this->collider->rigidbody.GetDirection();
		VECTOR aimVelocity = VScale(direction, this->speed);
		VECTOR prevVelocity = this->collider->rigidbody.GetVelocity();
		VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);
		this->collider->rigidbody.SetVelocity(newVelocity);
	}

	/*描画フラグが立っていたら*/
	if (this->isDraw)
	{
		//アニメーションの更新
		UpdateAnimation();
		float animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
		//もし走っているときにスタミナがなくなれば、再生速度を遅くする
		if (this->state->CheckFlag(this->MASK_RUN) && !CanAction(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]))
		{
			animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_ANIMATION_SPEED"];
		}
		VECTOR position = this->collider->rigidbody.GetPosition();
		//もしダウン中にアニメーション再生が終了していたら
		if (this->state->CheckFlag(this->DOWN) && this->animation->GetIsChangeAnim())return;
		if (this->isStopAnimation) return;
		//アニメーションの再生
		this->animation->Play(&this->modelHandle, this->nowAnimation, animationPlayTime);
	}
}

/// <summary>
/// 描画
/// </summary>
const void Player::DrawCharacterInfo()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& debug = Singleton<Debug>::GetInstance();

	VECTOR position = this->collider->rigidbody.GetPosition();
	VECTOR direction = this->collider->rigidbody.GetDirection();
	VECTOR rotation = this->collider->rigidbody.GetRotation();
	if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER))
	{
		auto& characterCollider = dynamic_cast<CharacterColliderData&> (*this->collider);
		printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f	\n", position.x, position.y, position.z);
		printfDx("PLAYER_DIRECTION X:%f,Y:%f,Z:%f	\n", direction.x, direction.y, direction.z);
		printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f	\n", rotation.x, rotation.y, rotation.z);
		printfDx("PLAYER_SPEED:%f					\n", this->speed);
		printfDx("%d:IDLE							\n", this->state->CheckFlag(this->IDLE));
		printfDx("%d:AVOID							\n", this->state->CheckFlag(this->AVOID));
		printfDx("%d:DEATH							\n", this->state->CheckFlag(this->DEATH));
		printfDx("%d:BLOCK							\n", this->state->CheckFlag(this->BLOCK));
		printfDx("%d:REACTION						\n", this->state->CheckFlag(this->REACTION));
		printfDx("%d:BLOCK_REACTION					\n", this->state->CheckFlag(this->BLOCK_REACTION));
		printfDx("%d:DOWN							\n", this->state->CheckFlag(this->DOWN));
		printfDx("%d:DOWN_UP						\n", this->state->CheckFlag(this->DOWN_UP));
		printfDx("%d:WALK_FRONT						\n", this->state->CheckFlag(this->WALK_FRONT));
		printfDx("%d:WALK_BACK						\n", this->state->CheckFlag(this->WALK_BACK));
		printfDx("%d:WALK_LEFT						\n", this->state->CheckFlag(this->WALK_LEFT));
		printfDx("%d:WALK_RIGHT						\n", this->state->CheckFlag(this->WALK_RIGHT));
		printfDx("%d:RUN_FRONT						\n", this->state->CheckFlag(this->RUN_FRONT));
		printfDx("%d:RUN_BACK						\n", this->state->CheckFlag(this->RUN_BACK));
		printfDx("%d:RUN_LEFT						\n", this->state->CheckFlag(this->RUN_LEFT));
		printfDx("%d:RUN_RIGHT						\n", this->state->CheckFlag(this->RUN_RIGHT));
		printfDx("%d:COMBO_1						\n", this->state->CheckFlag(this->COMBO_1));
		printfDx("%d:COMBO_2						\n", this->state->CheckFlag(this->COMBO_2));
		printfDx("%d:COMBO_3						\n", this->state->CheckFlag(this->COMBO_3));
		printfDx("%d:SKILL							\n", this->state->CheckFlag(this->SKILL));
		printfDx("%d:HEAL							\n", this->state->CheckFlag(this->HEAL));
		printfDx("%d:LOCK_ON						\n", this->isLockOn);
		printfDx("%d:REACTION_TYPE					\n", characterCollider.data->playerReaction);
		printfDx("DOT:%f							\n", this->dot);
		printfDx("DEG:%f							\n", this->deg);


	}
	
	/*かげの描画*/
	if (this->isDraw)
	{
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
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
}
/// <summary>
/// 速度の更新
/// </summary>
void Player::UpdateMoveVector()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*移動ベクトルを初期化する*/
	VECTOR direction = Gori::ORIGIN;
	VECTOR rotation = Gori::ORIGIN;
	/*移動しているときか回避しているときは移動ベクトルを出す*/
	if (this->state->CheckFlag(this->MASK_CAN_VELOCITY) || this->isBlockingMove)
	{
		rotation = this->moveVectorRotation;
		direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
		//direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));
		/*移動ベクトルを正規化*/
		direction = VNorm(direction);
	}

	/*新しいvelocityを出す*/
	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = this->collider->rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);
	
	/*走っていたらラープで補完する*/
	if (this->state->CheckFlag(this->MASK_RUN))
	{
		//補正値
		const VECTOR LERP_VALUE = Convert(json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_VELOCITY"]);
		VECTOR larpVelocity = Lerp(prevVelocity, aimVelocity, LERP_VALUE);
	}

	/*ダウンリアクションだったらvelocityを反対に向ける*/
	if (this->state->CheckFlag(this->DOWN))
	{
		newVelocity = VScale(newVelocity, -1.0f);
		newVelocity.y = prevVelocity.y;
	}

	/*velocityをセット*/
	this->collider->rigidbody.SetVelocity(newVelocity);
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
	if (this->isBlockingMove)
	{
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	}
	else if (this->state->CheckFlag(this->MASK_WALK))
	{
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
	}
	else if (this->state->CheckFlag(this->MASK_RUN))
	{
		if (CanAction(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]))
		{
			maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"];
		}
		else
		{
			maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
			this->animationPlayTime = 0.3f;
		}
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]);
	}


	VECTOR		nowRotation = this->collider->rigidbody.GetRotation();
	float		nowDeg		= (nowRotation.y * 180.0f / DX_PI_F);
	float		nextDeg		= (this->nextRotation.y * 180.0f / DX_PI_F);
	const float PI_2		= 360.0f;
	if (nextDeg < 0.0f) { nextDeg += PI_2; }
	float diff = nowDeg - nextDeg;
	if (diff < 0.0f) { diff *= -1.0f; }
	/*差が定数以上なら減速させる*/
	if (diff >= 180.0f)
	{
		if (!this->isDecSpeed)
		{
			maxSpeed = 0.0f;
			this->isDecSpeed = true;
		}
	}
	else
	{
		this->isDecSpeed = false;
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
/// 回転率の更新
/// </summary>
void Player::UpdateRotation()
{
	///*もし回避中だったら移動用回転率の向きに向ける*/
	//if (this->state->CheckFlag(this->MASK_AVOID))
	//{
	//	this->collider->rigidbody.SetRotation(this->moveVectorRotation);
	//	return;
	//}

	/*初期化*/
	const float PI = 180.0f;						//弧度法でのπ
	VECTOR		nowRotation = this->collider->rigidbody.GetRotation();	//回転率
	bool		isInputLStick = false;						//Lスティック入力
	VECTOR		cameraDirection = Gori::ORIGIN;	//カメラの向き
	VECTOR		playerToTargetDirection = Gori::ORIGIN;	//カメラの向き
	VECTOR		wasd = Gori::ORIGIN;	//wasd入力
	VECTOR		lStick = Gori::ORIGIN;	//lStick入力(上:Z+ 下:Z- 左:x- 右:x+)
	this->state->ClearFlag(this->MASK_MOVE);

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();


	/*移動できるか*/
	if (CanRotation())
	{
		/*パッド入力の取得*/
		int pad = input.GetPadState();

		/*スティック入力*/
		lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));

		/*走ったか*/
		bool isRun = (input.GetPadState() & PAD_INPUT_6);
		this->isBlockingMove = false;

			//スティック入力があるか
			if (lStick.x != 0.0f || lStick.z != 0.0f)
			{
				isInputLStick = true;
				if (this->state->CheckFlag(this->BLOCK))
				{
					this->isBlockingMove = true;
				}
				/*回避や攻撃をしていなければ移動状態を切り替える*/
				else if (!this->state->CheckFlag(this->MASK_ATTACK | this->MASK_AVOID))
				{
					unsigned int moveState = 0;
					//走っていたら
					if (isRun)
					{
						//moveState = this->whenRunMoveState[moveState];
						moveState = this->RUN_FRONT;
					}
					else
					{
						////ロックオンしていたら
						//if (this->isLockOn)
						//{
						//	//Xのほうが入力されている値が大きければ
						//	if (lStick.x * lStick.x > lStick.z * lStick.z)
						//	{
						//		//右
						//		if (lStick.x > 0.0f)
						//		{
						//			moveState = this->WALK_RIGHT;
						//		}
						//		//左
						//		else
						//		{
						//			moveState = this->WALK_LEFT;
						//		}
						//	}
						//	else
						//	{
						//		//前
						//		if (lStick.z < 0.0f)
						//		{
						moveState = this->WALK_FRONT;
						//		}
						//		//後ろ
						//		else
						//		{
						//			moveState = this->WALK_BACK;
						//		}
						//	}
						//}
						//else
						//{
						//	moveState = this->WALK_FRONT;
						//}
					}
					//状態のセット
					this->state->ClearFlag(this->MASK_REACTION);
					this->state->SetFlag(moveState);
				}
			}

		/*カメラの向きを出す*/
		//カメラ座標からプレイヤーの座標へのベクトルを出す
		cameraDirection = camera.GetCameraDirection();
		//求めたベクトルを正規化する
		cameraDirection = VNorm(cameraDirection);

		///*プレイヤーからボスへの向きを出す*/
		////ボス座標からプレイヤーの座標へのベクトルを出す
		//playerToTargetDirection = VSub(enemy.GetRigidbody().GetPosition(), this->collider->rigidbody.GetPosition());
		////求めたベクトルを正規化する
		//playerToTargetDirection = VNorm(playerToTargetDirection);


		/*カメラの向いている方向と、プレイヤーが最初に向いていた方向をもとにモデルの回転率を出す。*/
		if (isInputLStick)
		{
			//スティック入力を正規化
			lStick = VNorm(lStick);

			//if (this->isLockOn && !isRun)
			//{
			//	nextRotation.y = static_cast<float>(
			//		-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x)) - 90.0f * (DX_PI_F / 180.0f));
			//}
			//else
			//{
			this->nextRotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
			//}

			//this->moveVectorRotation.y = static_cast<float>(
			//	-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
			//	- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
			//this->moveVectorRotation = nextRotation;
			this->attackComboCount = -1;
		}

		/*スティック入力なければ早期リターン*/
		else return;
	}

	/*現在の回転率を出す*/
	VECTOR lerpValue = Convert(json.GetJson(JsonManager::FileType::PLAYER)["ROTATE_LERP_VALUE"]);
	nowRotation = Lerp360Angle(nowRotation, this->nextRotation, lerpValue);
	
	/*回転率のセット*/
	this->moveVectorRotation = nowRotation;
	this->collider->rigidbody.SetRotation(nowRotation);

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
const bool Player::GetIsAttack()const
{
	return this->state->CheckFlag(this->MASK_ATTACK);
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
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& effect = Singleton<EffectManager>  ::GetInstance();
	
	/*リアクションキャンセルの判定*/
	if (FrameCount(static_cast<int>(FrameCountType::REACTION_CANSEL), json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"]))
	{
		this->isCancelReaction = true;
	}

	/*ダウン中にアニメーションが終了していたら*/
	if (this->state->CheckFlag(this->DOWN) && this->animation->GetIsChangeAnim())
	{
		//シングルトンクラスのインスタンスの取得
		auto& input = Singleton<InputManager> ::GetInstance();
		//スティック入力
		VECTOR lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
		if (lStick.x != 0.0f || lStick.z != 0.0f)
		{
			this->state->ClearFlag(this->DOWN);
			this->state->SetFlag(this->DOWN_UP);
		}
	}

	/*リアクション中にアニメーションの再生が終了していたらフラグを下す*/
	else if (this->state->CheckFlag(this->MASK_REACTION) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ALL);
	}

	/*ヒット番号を取得*/
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);

	/*現在のヒット番号と前のヒット番号が異なっていたら*/
	if (collider.data->isHit)
	{
		//ガード中
		//内積が定数以内 & ガード中 & ガードに必要なスタミナが足りている
		if (this->state->CheckFlag(this->BLOCK) && collider.data->isGuard)
		{
			effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
			this->state->ClearFlag(this->MASK_ALL);
			this->state->SetFlag(this->BLOCK_REACTION);
			CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]);
			this->animation->SetAddRate(0.05f);
		}
		else
		{
			auto& data = dynamic_cast<PlayerData&>(*collider.data);
			//無敵じゃなければ
			if (!data.isInvinvible)
			{
				auto& effect = Singleton<EffectManager>::GetInstance();
				effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);

				this->state->ClearFlag(this->MASK_ALL);
				this->state->SetFlag(this->reactionMap[data.playerReaction]);
				this->speed = json.GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][data.playerReaction];
				this->hitStop->SetHitStop
				(
					data.hitStopTime,
					data.hitStopType,
					data.hitStopDelay,
					data.slowFactor
				);
			}

		}
		collider.data->isHit = false;
	}

	/*ダウン中なら無敵フラグを立てる*/
	if (this->state->CheckFlag(this->DOWN))
	{
		collider.data->isInvinvible = true;
		//コンボリセットフラグを立てる
		this->isCount[static_cast<int>(FrameCountType::REACTION_CANSEL)] = true;
	}
	else
	{
		collider.data->isInvinvible = false;
	}
	if (!this->state->CheckFlag(this->DOWN_UP))
	{
		this->isCancelReaction = false;
	}
}

/// <summary>
/// デス
/// </summary>
void Player::Death()
{
	if (!this->isAlive) return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);

	if (this->state->CheckFlag(this->DEATH))
	{
		if (this->animation->GetIsChangeAnim())
		{
			this->isAlive = false;
			this->isDraw = false;
		}
	}
	else
	{
		/*もしHPが０未満だったら*/
		if (collider.data->hp < 0)
		{
			this->state->ClearFlag(this->MASK_ALL);
			this->state->SetFlag(this->DEATH);
		}
	}
}

/// <summary>
/// ブロック
/// </summary>
void Player::Block()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input		= Singleton<InputManager> ::GetInstance();
	auto& json		= Singleton<JsonManager>  ::GetInstance();
	auto& collider	= dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data		= dynamic_cast<PlayerData&>(*collider.data);

	/*pad入力*/
	int pad = input.GetPadState();

	/*ブロックできるか*/
	if (!CanBlock())return;

	/*RTが押されたか*/
	if (pad & PAD_INPUT_8)
	{
		//敵のインスタンスを取得
		auto& enemy = Singleton<EnemyManager>::GetInstance();
		//敵とプレイヤーの向きの内積を計算し、定数以内ならガードが成功する
		VECTOR enemyFirstDirection	= Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);
		VECTOR playerFirstDirection = Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
		VECTOR enemyDirection		= VTransform(enemyFirstDirection, MGetRotY(enemy.GetRigidbody().GetRotation().y));
		VECTOR playerDirection		= VTransform(playerFirstDirection, MGetRotY(collider.rigidbody.GetRotation().y));
		float  TOLERANCE_DOT		= json.GetJson(JsonManager::FileType::PLAYER)["TOLERANCE_DOT"];
			   enemyDirection		= VNorm(enemyDirection);
			   playerDirection		= VNorm(playerDirection);
		this->dot = VDot(enemyDirection, playerDirection);
		this->dot = this->dot * 180.0f / DX_PI_F;
		if (this->dot < 0) { this->dot *= -1.0f; }

		//攻撃フラグを下す
		this->state->ClearFlag(this->MASK_ATTACK);
		this->state->SetFlag(this->BLOCK);

		if (this->dot >= TOLERANCE_DOT &&
			CanAction(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]))
		{
			if (!data.isGuard)
			{
				data.isGuard = true;
			}
		}
		else
		{
			data.isGuard = false;
		}
		this->animation->SetAddRate(0.3f);
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
		data.isGuard = false;
		this->animation->SetAddRate(0.05f);
	}
}

/// <summary>
/// 回復
/// </summary>
void Player::Heal()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager>	::GetInstance();
	auto& json = Singleton<JsonManager>		::GetInstance();
	auto& debug = Singleton<Debug>			::GetInstance();
	auto& effect = Singleton<EffectManager> ::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);

	/*無敵フラグが立っていたら最大HPから変えない*/
	if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER) && json.GetJson(JsonManager::FileType::DEBUG)["PLAYER_INVINCIBLE"])
	{
		data.hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
	}

	/*回復アニメーションが終了していたら*/
	if (this->state->CheckFlag(this->HEAL) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->HEAL);
	}

	/*pad入力*/
	int pad = input.GetPadState();
	bool isInputY = (pad & PAD_INPUT_1);

	/*回復していたら*/
	if (this->isCount[static_cast<int>(FrameCountType::HEAL)])
	{
		/*回復のインターバルを計算*/
		FrameCount(static_cast<int>(FrameCountType::HEAL), json.GetJson(JsonManager::FileType::PLAYER)["HEAL_INTERVAL_FRAME"]);
	}

	/*回復できるか*/
	if (!CanHeal()) return;

	/*前に回復していないかつXボタンが押されたら*/
	if (isInputY)
	{
		if (!this->state->CheckFlag(this->HEAL))
		{
			effect.OnIsEffect(EffectManager::EffectType::PLAYER_HEAL);
			this->state->ClearFlag(this->BLOCK);
			this->state->SetFlag(this->HEAL);
			data.hp += json.GetJson(JsonManager::FileType::PLAYER)["HEAL_VALUE"];
			this->healOrbNum--;
			this->isCount[static_cast<int>(FrameCountType::HEAL)] = true;
			//最大HPを超えないようにする
			if (data.hp >= json.GetJson(JsonManager::FileType::PLAYER)["HP"])
			{
				data.hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
			}
		}
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

	/*回避していたら*/
	if (this->state->CheckFlag(this->MASK_AVOID))
	{
		//アニメーションが終了していたら
		if (this->animation->GetIsChangeAnim())
		{
			this->state->ClearFlag(this->MASK_AVOID);
		}
	}

	/*フレーム回避用フレームカウントが最大に達していたら無敵フラグを下す*/
	if (FrameCount(static_cast<int>(FrameCountType::JUST_AVOID), json.GetJson(JsonManager::FileType::PLAYER)["JUST_AVOID_MAX_FRAME"]))
	{
		auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
		auto& data = dynamic_cast<PlayerData&>(*collider.data);
		data.isInvinvible = false;
	}

	/*回避時回転用フレームカウントの判定*/
	FrameCount(static_cast<int>(FrameCountType::AVOID_CAN_ROTATE), json.GetJson(JsonManager::FileType::PLAYER)["AVOID_CAN_ROTATE_FRAME"]);

	/*今回避できるか*/
	if (!CanRolling())return;

	/*消費スタミナは足りるのか*/
	if (!CanAction(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]))return;

	/*Aボタンが押された && 回避の無敵時間が終了している*/
	if ((pad & PAD_INPUT_3) && !this->isCount[static_cast<int>(FrameCountType::AVOID)])
	{
		//攻撃フラグと移動フラグを下す
		this->state->ClearFlag(this->MASK_ATTACK | this->MASK_MOVE | this->MASK_REACTION);
		//回避フラグを立てる
		this->state->SetFlag(this->AVOID);
		//回転可能フラグを立てる
		this->isCount[static_cast<int>(FrameCountType::AVOID_CAN_ROTATE)] = true;
		//フレーム回避フラグを立てる
		this->isCount[static_cast<int>(FrameCountType::JUST_AVOID)] = true;
		//無敵フラグを立てる
		auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
		auto& data = dynamic_cast<PlayerData&>(*collider.data);
		data.isInvinvible = true;
		//スタミナの計算
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]);
		//スピードに回避の初速を入れる
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["ROLLING_SPEED"];
	}

}
/// <summary>
/// 攻撃
/// </summary>
void Player::Attack()
{
	/*攻撃中にアニメーションが終了していたら攻撃フラグを下す*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ATTACK);
	}

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();
	auto& effect = Singleton<EffectManager>  ::GetInstance();

	const int COMBO_RESET = static_cast<int>(FrameCountType::COMBO_RESET);
	const int ATTACK_CANCEL = static_cast<int>(FrameCountType::ATTACK_CANCEL);
	const int ATTACK_CAN_ROTATE = static_cast<int>(FrameCountType::ATTACK_CAN_ROTATE);
	const int CHARGE_ATTACK = static_cast<int>(FrameCountType::CHARGE_ATTACK);

	/*コンボリセットフラグの判定*/
	if (FrameCount(COMBO_RESET, json.GetJson(JsonManager::FileType::PLAYER)["CONBO_RESET_MAX_FRAME"]))
	{
		this->attackComboCount = -1;
	}

	/*攻撃リセットフラグ*/
	if (this->attackComboCount != -1 && FrameCount(ATTACK_CANCEL, json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_CANCEL_MAX_FRAME"][this->attackComboCount]))
	{
		this->isCancelAttack = true;
	}

	/*回転可能フラグの判定*/
	if (FrameCount(ATTACK_CAN_ROTATE, json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_CAN_ROTATE_FRAME"]))
	{
	}


	/*pad入力*/
	int pad = input.GetPadState();
	bool isPushY = (pad & PAD_INPUT_2);
	bool isPushB = (pad & PAD_INPUT_4);
	this->isStopAnimation = false;

	/*攻撃をためている最中だったら*/
	if (this->isChargeAttack)
	{
		bool isStartAttack = false;
		//フレームを増加
		this->frameCount[CHARGE_ATTACK]++;
		//フレームが定数を超えていたらアニメーションを停止させる
		if (this->frameCount[CHARGE_ATTACK] >= json.GetJson(JsonManager::FileType::PLAYER)["CHARGE_ATTACK_STOP_FRAME"])
		{
			this->isStopAnimation = true;
			ResetFrameCount(ATTACK_CAN_ROTATE);
		}
		//フレームが定数を超えていたらチャージを終了させる
		if (this->frameCount[CHARGE_ATTACK] >= json.GetJson(JsonManager::FileType::PLAYER)["CHARGE_ATTACK_STOP_MAX_FRAME"])
		{
			isStartAttack = true;
		}
		//もし攻撃開始フラグが立っていたら攻撃を開始する
		if (isStartAttack)
		{
			//ダメージの計算
			int damageFactor = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE_FACTOR"];
			int baseDamage = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE"];
			int damage = baseDamage * (this->frameCount[CHARGE_ATTACK] / damageFactor);
			//ベースよりも小さくならないようにする
			if (damage < baseDamage)
			{
				damage = baseDamage;
			}
			//ダメージのセット
			attack.SetDamage(damage);
			//攻撃の開始
			attack.OnIsStart();
			//関係変数の初期化
			this->isStopAnimation = false;
			this->isChargeAttack = false;
			ResetFrameCount(CHARGE_ATTACK);
		}
	}

	/*攻撃できるか*/
	if (!CanAttack())return;

	/*攻撃ボタンが押されていたら早期リターン*/
	if (!isPushB && !isPushY) return;

	/*回避とリアクションフラグを下す*/
	this->state->ClearFlag(this->MASK_AVOID | this->MASK_REACTION | this->MASK_ATTACK);

	/*攻撃cancelフラグを下す*/
	this->isCancelAttack = false;

	//コンボリセットフラグを下す
	ResetFrameCount(COMBO_RESET);
	//回転可能フレームを初期化
	ResetFrameCount(ATTACK_CAN_ROTATE);
	this->isCount[ATTACK_CAN_ROTATE] = true;

	int attackType;
	/*Bが押されたか*/
	if (isPushB)
	{
		//コンボ数を増やす
		this->attackComboCount++;
		//コンボが最大を超えたら1に戻す
		if (this->attackComboCount > static_cast<int>(AttackType::COMBO_3))
		{
			this->attackComboCount = static_cast<int>(AttackType::COMBO_1);
		}
		//コンボ数が2以下だったら
		if (this->attackComboCount <= static_cast<int>(AttackType::COMBO_2))
		{
			//攻撃キャンセルフラグを立てる
			this->isCount[ATTACK_CANCEL] = true;
		}
		//コンボリセットフラグを立てる
		this->isCount[COMBO_RESET] = true;
		//コンボ数をもとに、攻撃の種類を設定
		attackType = this->attackComboCount;
		//ダメージのセット
		attack.SetDamage(json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][this->attackComboCount]);
		//攻撃フラグを立てる
		attack.OnIsStart();
	}

	/*Yが押されたか*/
	else if (isPushY)
	{
		//攻撃の種類を設定
		attackType = static_cast<int>(AttackType::SKILL);
		//攻撃キャンセルフラグを下す
		ResetFrameCount(ATTACK_CANCEL);
		//コンボをリセットする
		this->attackComboCount = -1;
		this->isCount[CHARGE_ATTACK] = true;
		this->isChargeAttack = true;
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_CHARGE);
	}

	/*攻撃フラグをセット*/
	this->state->SetFlag(this->attackMap[attackType]);

}

/// <summary>
/// フレームカウント
/// </summary>
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

/// <summary>
/// フレームカウントの初期化
/// </summary>
void Player::ResetFrameCount(const int _index)
{
	this->isCount[_index] = false;
	this->frameCount[_index] = 0;
}


const bool Player::CanRotation()const
{
	if (this->state->CheckFlag(this->MASK_REACTION) && !this->isCancelReaction)	return false;//リアクション
	if (this->state->CheckFlag(this->DEATH)	)			return false;//デス
	if (this->state->CheckFlag(this->MASK_AVOID) && !this->isCount[static_cast<int>(FrameCountType::AVOID_CAN_ROTATE)])		return false;//回避
	if (this->state->CheckFlag(this->MASK_ATTACK) && !this->isCount[static_cast<int>(FrameCountType::ATTACK_CAN_ROTATE)])		return false;//攻撃
	//if (this->state->CheckFlag(this->BLOCK))			return false;//防御
	if (this->state->CheckFlag(this->HEAL))				return false;//回復
	return true;
}
const bool Player::CanRolling()const
{
	//if (this->state->CheckFlag(this->MASK_ATTACK))		return false;//攻撃
	if (this->state->CheckFlag(this->BLOCK))									return false;//ブロック
	if (this->state->CheckFlag(this->MASK_REACTION) && !this->isCancelReaction)	return false;//リアクション
	if (this->state->CheckFlag(this->DEATH))									return false;//死亡
	if (this->state->CheckFlag(this->MASK_AVOID))								return false;//回避
	if (this->state->CheckFlag(this->HEAL))										return false;//回復

	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->MASK_REACTION))						return false;//リアクション
	if (this->state->CheckFlag(this->DEATH))								return false;//デス
	if (this->state->CheckFlag(this->MASK_ATTACK) && !this->isCancelAttack)	return false;//攻撃
	if (this->state->CheckFlag(this->HEAL))									return false;//回復
	if (this->state->CheckFlag(this->MASK_AVOID))							return false;//回避
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->MASK_REACTION))						return false;//リアクション
	if (this->state->CheckFlag(this->DEATH))								return false;//デス
	if (this->state->CheckFlag(this->MASK_AVOID))							return false;//回避
	if (this->state->CheckFlag(this->MASK_ATTACK) && !this->isCancelAttack)	return false;//攻撃
	if (this->state->CheckFlag(this->HEAL))									return false;//回復
	return true;
}
const bool Player::CanHeal()const
{
	if (this->state->CheckFlag(this->MASK_REACTION))		return false;//リアクション
	if (this->state->CheckFlag(this->DEATH))				return false;//デス
	if (this->state->CheckFlag(this->MASK_AVOID))			return false;//回避
	if (this->state->CheckFlag(this->MASK_ATTACK))			return false;//攻撃
	if (this->healOrbNum == 0)									return false;
	if (this->isCount[static_cast<int>(FrameCountType::HEAL)])	return false;

	return true;
}
const bool Player::DontAnyAction()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))		return false;//攻撃
	if (this->state->CheckFlag(this->MASK_REACTION))	return false;//リアクション
	if (this->state->CheckFlag(this->MASK_AVOID))		return false;//回避
	if (this->state->CheckFlag(this->DEATH))			return false;//ブロック
	if (this->state->CheckFlag(this->MASK_MOVE))		return false;//移動
	if (this->state->CheckFlag(this->BLOCK))			return false;//ジャンプ
	if (this->state->CheckFlag(this->HEAL))				return false;//回復

	return true;
}

/// <summary>
/// 必要な消費スタミナと現在のスタミナの残量を比べてアクションが可能ならtrueを返す
/// </summary>
/// <returns></returns>
const bool Player::CanAction(const float _staminaConsumed)const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);
	float staminaConsumed = _staminaConsumed * -1.0f;
	/*スタミナの消費量が現在のスタミナの総量よりも多ければfalseを返す*/
	if (staminaConsumed > data.stamina)return false;
	return true;
}

/// <summary>
/// スタミナの回復処理
/// </summary>
void Player::CalcStamina(const float _staminaConsumed)
{
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);

	/*それ以外の状態だったら状態に応じてスタミナを消費する*/
	//走り
	data.stamina += _staminaConsumed;
	/*上限値、下限値を超えないように調整*/
	if (data.stamina >= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"])
	{
		data.stamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	}
	else if (data.stamina < 0)
	{
		data.stamina = 0;
	}
}

/// <summary>
/// スタミナの取得
/// </summary>
const int Player::GetStamina()const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);
	return data.stamina;
}

/// <summary>
/// ロックオン
/// </summary>
void Player::LockOn()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = Singleton<InputManager>  ::GetInstance();

	/*右スティック押し込みがあったか*/
	if ((input.GetPadState() & PAD_INPUT_10))
	{
		if (!this->isCount[static_cast<int>(FrameCountType::LOCK_ON)])
		{
			if (this->isLockOn)
			{
				this->isLockOn = false;
			}
			else
			{
				this->isLockOn = true;
			}
			this->isCount[static_cast<int>(FrameCountType::LOCK_ON)] = true;
		}
	}
	else
	{
		this->isCount[static_cast<int>(FrameCountType::LOCK_ON)] = false;
	}
}

