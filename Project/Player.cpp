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
#include "Satellite.h"
#include "Shield.h"
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
	, state				(nullptr)
	, shield			(nullptr)
	, collider			(nullptr)
	, moveVector		{ 0.0f, 0.0f, 0.0f }
	, direction			{ 0.0f, 0.0f, 0.0f }
	, fixVector			{ 0.0f, 0.0f, 0.0f }
	, moveVectorRotation{ 0.0f, 0.0f, 0.0f }
	, wasd				{ 0.0f, 0.0f, 0.0f }
	, lStick			{ 0.0f, 0.0f, 0.0f }
	, velocity			(0.0f)
	, isShot			(false)
	, isShowElementWheel(false)
	, elementAngle		(0.0f)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*インスタンスの作成*/
	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::PLAYER));
	this->shield = new Shield();
	this->state = new BitFlag();
	this->collider = new Collider();
	for (int i = 0; i < this->SATELLITE_NUM; i++)
	{
		this->satellite[i] = new Satellite();
	}

	/*vectorの追加*/
	this->frameCount.emplace_back(0);
	this->isCount.emplace_back(false);

	/*アニメーションの設定*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);
	this->animationMap.emplace(this->IDLE		 , static_cast<int>(AnimationType::IDLE));
	this->animationMap.emplace(this->WALK		 , static_cast<int>(AnimationType::WALK));
	this->animationMap.emplace(this->RUN		 , static_cast<int>(AnimationType::RUN));
	this->animationMap.emplace(this->AVOID		 , static_cast<int>(AnimationType::AVOID));
	this->animationMap.emplace(this->JUMP		 , static_cast<int>(AnimationType::JUMP));
	this->animationMap.emplace(this->FLAME_ATTACK, static_cast<int>(AnimationType::FLAME_ATTACK));
	this->animationMap.emplace(this->BOLT_ATTACK , static_cast<int>(AnimationType::BOLT_ATTACK));
	this->animationMap.emplace(this->ICE_ATTACK	 , static_cast<int>(AnimationType::ICE_ATTACK));
	this->animationMap.emplace(this->EARTH_ATTACK, static_cast<int>(AnimationType::EARTH_ATTACK));
	this->animationMap.emplace(this->STORM_ATTACK, static_cast<int>(AnimationType::STORM_ATTACK));
	this->attackTypeMap.emplace(static_cast<int>(Element::FRAME), this->FLAME_ATTACK);
	this->attackTypeMap.emplace(static_cast<int>(Element::BOLT), this->BOLT_ATTACK);
	this->attackTypeMap.emplace(static_cast<int>(Element::ICE), this->ICE_ATTACK);
	this->attackTypeMap.emplace(static_cast<int>(Element::EARTH), this->EARTH_ATTACK);
	this->attackTypeMap.emplace(static_cast<int>(Element::STORM), this->STORM_ATTACK);
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	for (int i = 0; i < this->SATELLITE_NUM; i++)
	{
		DeleteMemberInstance(this->satellite[i]);
	}
	DeleteMemberInstance(this->shield);
	DeleteMemberInstance(this->collider);
	this->frameCount.clear();
	this->isCount.clear();
	this->animationMap.clear();
	this->attackTypeMap.clear();
}

void Player::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR position = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//座標
	const VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//回転率
	const VECTOR scale = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //拡大率
	float ANGLE_OFFSET = json.GetJson(JsonManager::FileType::PLAYER)["SATELLITE_ANGLE_OFFSET"];
	this->velocity = 0.0f;
	this->direction = VGet(0.0f, 0.0f, -1.0f);

	/*モデルのトランスフォームの設定*/
	this->model->SetTransform(position, rotation, scale);

	this->shield->Initialize();

	satellite[0]->Initialize(0.0f,position, Convert(json.GetJson(JsonManager::FileType::PLAYER)["SATELLITE_POSITION_OFFSET_A"]));
	satellite[1]->Initialize(180.0f,position, Convert(json.GetJson(JsonManager::FileType::PLAYER)["SATELLITE_POSITION_OFFSET_A"]));
}

/// <summary>
/// アクション
/// </summary>
void Player::Action()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	UpdateElement();

	/*回転率の更新*/
	UpdateRotation();

	/*移動*/
	Move();

	/*ジャンプ*/
	Jump();

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
	const float  radius = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];	 //

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

	if (this->state->CheckFlag(this->MASK_MOVE))
	{
		satellite[0]->Update(true, VNorm(this->direction), this->model->GetPosition());
		satellite[1]->Update(false, VNorm(this->direction), this->model->GetPosition());
	}
	this->shield->Update(this->model->GetPosition());

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
	printfDx("ELEMENT_ANGLE:%f\n", this->elementAngle);

	this->model->Draw();
	for (int i = 0; i < this->SATELLITE_NUM; i++)
	{
		satellite[i]->Draw();
	}
	DrawLine(950, 540, 970, 541, GetColor(200, 200, 200), 2);
	DrawLine(960, 530, 961, 550, GetColor(200, 200, 200), 2);

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
	if (!CanMove())return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();

	/*スティック入力がなければ移動フラグを下してreturnを返す*/
	if (this->lStick.x == 0 && this->lStick.z == 0 && this->wasd.x == 0 && this->wasd.z == 0)
	{
		//移動状態を削除する
		this->state->ClearFlag(this->MASK_MOVE);
	}
	else
	{
		/*移動状態を削除する*/
		this->state->ClearFlag(this->MASK_MOVE);
		
		/*移動状態をセット*/
		//走るか
		if (input.GetPadState() & PAD_INPUT_6 || CheckHitKey(KEY_INPUT_LSHIFT))
		{
			this->state->SetFlag(this->RUN);
		}
		else
		{
			this->state->SetFlag(this->WALK);
		}
	}

	/*速度の更新*/
	UpdateVelocity();

	/*移動ベクトルの更新*/
	UpdateMoveVector();

	/*向きの更新*/
	if (!this->isShot)
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

	/*もしエイム状態だったら*/
	if (this->state->CheckFlag(this->MASK_ATTACK))
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

	if (isInputKey || isInputLStick || this->isShot)
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

	
	/*攻撃できるか*/
	if (CanAttack())
	{
		/*攻撃関係の初期化*/
		this->state->ClearFlag(this->MASK_ATTACK);
		this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = false;
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;


		/*XボタンまたはキーボードのXが押されたら*/
		if (input.GetPadState() & PAD_INPUT_1 || CheckHitKey(KEY_INPUT_X))
		{
			this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = true;
			unsigned int flag = this->attackTypeMap[this->element];
			this->state->SetFlag(flag);
		}
	}
	else
	{
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)]++;
		if (this->model->GetIsChangeAnim())
		{
			this->state->ClearFlag(this->MASK_ATTACK);
			this->state->SetFlag(this->IDLE);
		}
	}
}


/// <summary>
/// 空を飛ぶ
/// </summary>
void Player::Jump()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*空を飛べるか*/
	if (!CanJump())return;

	/*変数の準備*/
	float y = json.GetJson(JsonManager::FileType::PLAYER)["GRAVITY"];

	/*BボタンまたはキーボードのBが押されたら*/
	if (!this->state->CheckFlag(this->JUMP))
	{
		if (input.GetPadState() & PAD_INPUT_4 || CheckHitKey(KEY_INPUT_SPACE))
		{
			this->jumpPower = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"];
			/*移動状態をセット*/
			this->state->SetFlag(this->JUMP);
		}
	}
	else
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
/// 移動できるか
/// </summary>
const bool Player::CanMove()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;
	return true;
}

/// <summary>
/// 攻撃できるか
/// </summary>
const bool Player::CanAttack()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*攻撃待機用フレームカウントフラグが立っていなかったらtrueを返す*/
	if (!this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)])return true;

	/*攻撃ごとに最大フレームカウントを決める*/
	int maxFrameCount = 0;
	if (this->state->CheckFlag(this->FLAME_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_INTERVAL"];
	}
	else if (this->state->CheckFlag(this->BOLT_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_INTERVAL"];
	}
	else if (this->state->CheckFlag(this->ICE_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_INTERVAL"];
	}
	else if (this->state->CheckFlag(this->EARTH_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["EARTH_MAGIC_INTERVAL"];
	}
	else if (this->state->CheckFlag(this->STORM_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_INTERVAL"];
	}

	/*もしフレームカウントが定数以上だったらtrueを返す*/
	if (this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] >= maxFrameCount)return true;

	return false;
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
/// 空を飛べるか
/// </summary>
const bool Player::CanJump()const
{
	return true;
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
/// 属性の更新
/// </summary>
void Player::UpdateElement()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*属性ホイールを表示するか*/
	if (input.GetPadState() & PAD_INPUT_5)
	{
		this->isShowElementWheel = true;
	}
	else
	{
		//falseなら早期リターン
		this->isShowElementWheel = false;
		return;
	}

	/*右スティックの傾きをもとに、属性を変更する*/
	vector<int> buf = { input.GetRStickState().XBuf ,input.GetRStickState().YBuf };
	this->rStick = buf;
	/*スティック入力がなければリターンを返す*/
	if (this->rStick[0] == 0 && this->rStick[1] == 0)return;
	this->elementAngle = (- atan2(this->rStick[1], this->rStick[0]) * 180.0f / DX_PI_F) - 90.0f;
	if (this->elementAngle < 0)
	{
		this->elementAngle += 360.0f;
		if (this->elementAngle == 360.0f)
		{
			this->elementAngle = 0.0f;
		}
	}

	if (31.0f >= this->elementAngle || this->elementAngle >= 320.0f)
	{
		this->element = static_cast<int>(Element::ICE);
	}
	else if (103.0f >= this->elementAngle && this->elementAngle >= 32.0f)
	{
		this->element = static_cast<int>(Element::BOLT);
	}
	else if (175.0f >= this->elementAngle && this->elementAngle >= 104.0f)
	{
		this->element = static_cast<int>(Element::FRAME);
	}
	else if (247.0f >= this->elementAngle && this->elementAngle >= 176.0f)
	{
		this->element = static_cast<int>(Element::STORM);
	}
	else if (319.0f >= this->elementAngle && this->elementAngle >= 248.0f)
	{
		this->element = static_cast<int>(Element::EARTH);
	}
}

/// <summary>
/// アニメーションの更新
/// </summary>
void Player::UpdateAnimation()
{
	/*攻撃していたら*/
	if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		//炎
		if (this->state->CheckFlag(this->FLAME_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::FLAME_ATTACK);
		}
		//雷
		else if (this->state->CheckFlag(this->BOLT_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::BOLT_ATTACK);
		}
		//氷
		else if (this->state->CheckFlag(this->ICE_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::ICE_ATTACK);
		}
		//土
		else if (this->state->CheckFlag(this->EARTH_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::EARTH_ATTACK);
		}
		//風
		else if (this->state->CheckFlag(this->STORM_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::STORM_ATTACK);
		}

		return;
	}

	/*回避していたら*/
	if (this->state->CheckFlag(this->AVOID))
	{
		this->nowAnimation = static_cast<int>(AnimationType::AVOID);
		return;
	}
	/*ジャンプしていたら*/
	if (this->state->CheckFlag(this->JUMP))
	{
		this->nowAnimation = static_cast<int>(AnimationType::JUMP);
		return;
	}

	/*移動していたら*/
	if (this->state->CheckFlag(this->MASK_MOVE))
	{
		//走り
		if (this->state->CheckFlag(this->RUN))
		{
			this->nowAnimation = static_cast<int>(AnimationType::RUN);
		}
		//歩き
		if (this->state->CheckFlag(this->WALK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::WALK);
		}
		return;
	}
	/*何もしていなかったら待機状態*/
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
}