#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Animation.h"
#include "Model.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "Boss.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "Vector4.h"
#include "Collider.h"

/// <summary>
/// コンストラクタ
/// </summary>
Boss::Boss()
	: model		(nullptr)
	, state		(nullptr)
	, moveVector{ 0.0f, 0.0f, 0.0f }
	, moveTarget{ 0.0f, 0.0f, 0.0f }
	, velocity	(0.0f)
	, isHitAttack(false)
	, isDraw(true)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->model		= new Model(asset.GetModel(LoadingAsset::ModelType::ENEMY));
	this->state		= new BitFlag();
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
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);

	/*関数マップの設定*/
	auto idleSet	  = [this]() {Idle(); };
	auto tauntSet	  = [this]() {Taunt(); };
	auto moveSet	  = [this]() {Move(); };
	auto reactionSet  = [this]() {Reaction(); };
	auto deathSet	  = [this]() {Death(); };
	auto restSet	  = [this]() {Rest(); };
	auto attackSet	  = [this]() {Attack(); };
	this->AddItemFunction(this->IDLE					, idleSet);
	this->AddItemFunction(this->TAUNT					, tauntSet);
	this->AddItemFunction(this->WALK_FRONT				, moveSet);
	this->AddItemFunction(this->WALK_LEFT				, moveSet);
	this->AddItemFunction(this->WALK_RIGHT				, moveSet);
	this->AddItemFunction(this->VERTICAL_SLASH			, attackSet);
	this->AddItemFunction(this->HORIZONTAL_SLASH		, attackSet);
	this->AddItemFunction(this->ROTATION_SLASH			, attackSet);
	this->AddItemFunction(this->KNOCK_UP_SLASH			, attackSet);
	this->AddItemFunction(this->STRONG_HORIZONTAL_SLASH	, attackSet);
	this->AddItemFunction(this->TWO_COMBO				, attackSet);
	this->AddItemFunction(this->THREE_COMBO				, attackSet);
	this->AddItemFunction(this->REACTION				, reactionSet);
	this->AddItemFunction(this->DEATH					, deathSet);
	this->AddItemFunction(this->REST					, restSet);
	/*アニメーションマップの設定*/
	this->stateAnimationMap.emplace(this->IDLE					  , static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->TAUNT					  , static_cast<int>(AnimationType::TAUNT));
	this->stateAnimationMap.emplace(this->WALK_FRONT			  , static_cast<int>(AnimationType::WALK_FRONT));
	this->stateAnimationMap.emplace(this->WALK_LEFT				  , static_cast<int>(AnimationType::WALK_LEFT));
	this->stateAnimationMap.emplace(this->WALK_RIGHT			  , static_cast<int>(AnimationType::WALK_RIGHT));
	this->stateAnimationMap.emplace(this->VERTICAL_SLASH		  , static_cast<int>(AnimationType::VERTICAL_SLASH));
	this->stateAnimationMap.emplace(this->HORIZONTAL_SLASH		  , static_cast<int>(AnimationType::HORIZONTAL_SLASH));
	this->stateAnimationMap.emplace(this->ROTATION_SLASH		  , static_cast<int>(AnimationType::ROTATION_SLASH));
	this->stateAnimationMap.emplace(this->KNOCK_UP_SLASH		  , static_cast<int>(AnimationType::KNOCK_UP_SLASH));
	this->stateAnimationMap.emplace(this->STRONG_HORIZONTAL_SLASH , static_cast<int>(AnimationType::STRONG_HORIZONTAL_SLASH));
	this->stateAnimationMap.emplace(this->TWO_COMBO				  , static_cast<int>(AnimationType::TWO_COMBO));
	this->stateAnimationMap.emplace(this->THREE_COMBO			  , static_cast<int>(AnimationType::THREE_COMBO));
	this->stateAnimationMap.emplace(this->REACTION				  , static_cast<int>(AnimationType::REACTION));
	this->stateAnimationMap.emplace(this->DEATH					  , static_cast<int>(AnimationType::DEATH));
	this->stateAnimationMap.emplace(this->REST					  , static_cast<int>(AnimationType::IDLE));

	this->state->SetFlag(this->IDLE);

}

/// <summary>
/// デストラクタ
/// </summary>
Boss::~Boss()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	for (int i = 0; i < this->COLLIDER_NUM; i++)
	{
		DeleteMemberInstance(this->collider[i]);
	}
	this->stateFunctionMap.clear();
	this->stateAnimationMap.clear();
	this->frameCount.clear();
	this->isCount.clear();
}

void Boss::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //拡大率

	this->velocity = 0.0f;
	this->hp = 1000;
	this->damage = 5;
	VECTOR toPlayer = VNorm(VSub(VGet(0.0f, 0.0f, 0.0f), this->model->GetPosition()));

	/*モデルのトランスフォームの設定*/
	this->model->SetTransform(POSITION, ROTATION, SCALE);
}

/// <summary>
/// 更新
/// </summary>
void Boss::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	VECTOR position = this->model->GetPosition();
	position = VAdd(position, this->moveVector);
	this->model->SetPosition(position);

	/*アニメーションの更新*/

	this->nowAnimation = this->stateAnimationMap[this->state->GetFlag()];
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->PlayAnimation(this->nowAnimation, this->animationPlayTime);
	/*コライダーの設定*/
	const float HEIGHT = json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	const float RADIUS = json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	this->collider[static_cast<int>(ColliderType::CHARACTER)]->SetCapsule(position, HEIGHT, RADIUS);
}

/// <summary>
/// アクション
/// </summary>
void Boss::Action()
{
	/*移動ベクトルの初期化*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*回転率を出す*/
	UpdateRotation();

	/*移動速度の更新*/
	UpdateVelocity();

	/*移動ベクトルを出す*/
	UpdateMoveVector();

	/*状態の切り替え*/
	ChangeState();

	/*状態ごとの処理を実行*/
	unsigned int flag = this->state->GetFlag();
	this->stateFunctionMap[flag].update();
}

/// <summary>
/// 咆哮
/// </summary>
void Boss::Taunt()
{
	/*咆哮中にアニメーションが終了していたらフラグを下す*/
	if (this->state->CheckFlag(this->TAUNT) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->TAUNT);
	}
}
/// <summary>
/// 移動
/// </summary>
void Boss::Move()
{
	
}

/// <summary>
/// ヒットリアクション
/// </summary>
void Boss::Reaction()
{

}
/// <summary>
/// 休憩
/// </summary>
void Boss::Rest()
{

}
/// <summary>
/// 死亡
/// </summary>
void Boss::Death()
{

}
/// <summary>
/// 攻撃
/// </summary>
void Boss::Attack()
{
	//	/*シングルトンクラスのインスタンスの取得*/
	//	auto& json = Singleton<JsonManager>::GetInstance();
	//auto& player = Singleton<PlayerManager>::GetInstance();

	///*待機状態だったら新しく攻撃を設定する*/
	//if (this->state->CheckFlag(this->IDLE))
	//{
	//	/*攻撃の種類をランダムで出す*/
	//	//int attackType = GetRand(static_cast<int>(INHIBITION));
	//	this->attackType = 1;
	//	this->state->ClearFlag(this->IDLE);
	//	this->state->SetFlag(this->attackTypeMap[this->attackType]);
	//	this->waitingCountBeforeAttack = 0;
	//	this->attackCount = 0;
	//	this->moveTarget = player.GetPosition();
	//}
	///*設定された攻撃ごとの関数を実行*/
	////this->attackFunctionMap[this->attackType];
	//switch (this->attackType)
	//{
	//case static_cast<int>(AttackType::RUSH):
	//	RushAttack();
	//	break;
	//case static_cast<int>(AttackType::JUMP):
	//	JumpAttack();
	//	break;
	//case static_cast<int>(AttackType::LASER):
	//	break;
	//case static_cast<int>(AttackType::SPIN):
	//	break;
	//case static_cast<int>(AttackType::INHIBITION):
	//	break;
	//}
}
void Boss::ChangeState()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	if (this->hp < 0)
	{
		this->state->SetFlag(this->DEATH);
		if (this->model->GetIsChangeAnim())
		{
			this->isDraw = false;
		}
	}

	/*デスしていなければ*/
	if (this->state->CheckFlag(this->DEATH))return;

	/*休憩するか*/
	if (CanRest())
	{
		this->state->ClearFlag(this->MASK_ALL);
		this->state->SetFlag(this->REST);
		this->isCount[static_cast<int>(FrameCountType::REST)] = true;
	}

	/*休憩中ならカウントを進める*/
	if (this->state->CheckFlag(this->REST))
	{
		/*もしカウントが終了していたらフラグを下す*/
		if (FrameCount(static_cast<int>(FrameCountType::REST), json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]))
		{
			this->state->ClearFlag(this->REST);
			this->state->SetFlag(this->IDLE);
		}
	}

	/*休憩中じゃなければ早期リターン*/
	if (this->state->CheckFlag(this->REST) || this->state->CheckFlag(this->MASK_ATTACK))return;

	/*移動するか*/
	const float TARGET_DISTANCE = VSize(VSub(player.GetPosition(), this->model->GetPosition()));//プレイヤーとの距離を求める
	const float MAX_TARGET_DISTANCE = json.GetJson(JsonManager::FileType::ENEMY)["TARGET_DISTANCE"];//目標との最大距離
	//もしプレイヤーとの距離が最大距離以上離れていたら追跡する
	if (TARGET_DISTANCE >= MAX_TARGET_DISTANCE)
	{
		this->state->ClearFlag(this->MASK_MOVE | this->IDLE);
		this->state->SetFlag(this->WALK_FRONT);
	}

	/*攻撃するか*/
	else
	{
		/*攻撃コライダー用変数*/
		float radius = 0.0f;
		float offsetScale = 0.0f;
		float offsetY = 0.0f;
		VECTOR position = { 0.0f,0.0f,0.0f };
		int attackType = static_cast<int>(AttackType::NONE);
		this->state->ClearFlag(this->MASK_MOVE | this->IDLE);

		/*phase1*/
		//今向いている方向とプレイヤーへの咆哮のない席が一定以上だったら回転切りをする
		VECTOR toPlayer = VNorm(VSub(player.GetPosition(),this->model->GetPosition()));
		this->dot = VDot(this->direction, toPlayer);
		this->dot = this->dot * 180.0f / DX_PI_F;
		if (this->dot < 0.0f)
		{
			this->state->SetFlag(this->ROTATION_SLASH);
			attackType = static_cast<int>(AttackType::ROTATION_SLASH);
		}
		//以前の攻撃が当たっていたらコンボ攻撃をする
		else if (this->isHitAttack)
		{
			this->state->SetFlag(this->TWO_COMBO);
			attackType = static_cast<int>(AttackType::TWO_COMBO);
		}
		//縦切りか横切りはランダムで決める
		else
		{
			int type = GetRand(1);
			if (type == 0)
			{
				this->state->SetFlag(this->VERTICAL_SLASH);
				attackType = static_cast<int>(AttackType::VERTICAL_SLASH);
			}
			else
			{
				this->state->SetFlag(this->HORIZONTAL_SLASH);
				attackType = static_cast<int>(AttackType::HORIZONTAL_SLASH);
			}
		}


		/*phase2*/
		

		/*コライダーの更新*/
		if (attackType != static_cast<int>(AttackType::NONE))
		{
			radius = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][attackType];
			offsetScale = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_SCALE"][attackType];
			offsetY = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"][attackType];
			position = this->model->GetPosition() + VScale(this->direction, offsetScale);
			position.y += offsetY;
			this->damage = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][attackType];
			this->attackNumber++;
		}
		this->collider[static_cast<int>(ColliderType::ATTACK)]->SetSphere(position, radius);
	}
}

/// <summary>
/// 描画
/// </summary>
const void Boss::Draw()const
{
	//VECTOR position = this->model->GetPosition();
	//VECTOR rotation = this->model->GetRotation();
	//printfDx("Boss_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	//printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	//printfDx("%f:Boss_DOT\n", this->dot);
	//printfDx("%d:IDLE					\n", this->state->CheckFlag(this->IDLE));
	//printfDx("%d:TAUNT					\n", this->state->CheckFlag(this->TAUNT));
	//printfDx("%d:WALK_FRONT				\n", this->state->CheckFlag(this->WALK_FRONT));
	//printfDx("%d:WALK_LEFT				\n", this->state->CheckFlag(this->WALK_LEFT));
	//printfDx("%d:WALK_RIGHT				\n", this->state->CheckFlag(this->WALK_RIGHT));
	//printfDx("%d:VERTICAL_SLASH			\n", this->state->CheckFlag(this->VERTICAL_SLASH));
	//printfDx("%d:HORIZONTAL_SLASH		\n", this->state->CheckFlag(this->HORIZONTAL_SLASH));
	//printfDx("%d:ROTATION_SLASH			\n", this->state->CheckFlag(this->ROTATION_SLASH));
	//printfDx("%d:KNOCK_UP_SLASH			\n", this->state->CheckFlag(this->KNOCK_UP_SLASH));
	//printfDx("%d:STRONG_HORIZONTAL_SLASH\n", this->state->CheckFlag(this->STRONG_HORIZONTAL_SLASH));
	//printfDx("%d:TWO_COMBO				\n", this->state->CheckFlag(this->TWO_COMBO));
	//printfDx("%d:STRONG_TWO_COMBO		\n", this->state->CheckFlag(this->STRONG_TWO_COMBO));
	//printfDx("%d:THREE_COMBO			\n", this->state->CheckFlag(this->THREE_COMBO));
	//printfDx("%d:REACTION				\n", this->state->CheckFlag(this->REACTION));
	//printfDx("%d:DEATH					\n", this->state->CheckFlag(this->DEATH));
	//printfDx("%d:REST					\n", this->state->CheckFlag(this->REST));

	if (this->isDraw)
	{
		this->model->Draw();
	}
	this->collider[static_cast<int>(ColliderType::CHARACTER)]->DrawHitCapsule();
	this->collider[static_cast<int>(ColliderType::ATTACK)]->DrawHitSphere();
}

/// <summary>
/// 速度の更新
/// </summary>
void Boss::UpdateMoveVector()
{
	VECTOR rotation = this->model->GetRotation();

	/*回転率をもとに移動ベクトルを出す*/
	this->moveVector = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));

	/*移動ベクトルを正規化*/
	this->moveVector = VNorm(this->moveVector);

	/*移動ベクトルの方向を代入*/
	this->direction = this->moveVector;

	/*移動ベクトルに速度を加算*/
	this->moveVector = VScale(this->moveVector, this->velocity);

}
/// <summary>
/// 速度の更新
/// </summary>
void Boss::UpdateVelocity()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->state->CheckFlag(this->MASK_MOVE))
	{
		this->velocity = json.GetJson(JsonManager::FileType::ENEMY)["VELOCITY"];
	}
	else
	{
		this->velocity = 0.0f;
	}
}
/// <summary>
/// 座標のgetter
/// </summary>
/// <returns></returns>
const VECTOR Boss::GetPosition()const
{
	return this->model->GetPosition();
}

/// <summary>
/// 回転率の更新
/// </summary>
void Boss::UpdateRotation()
{
	/*回転できなければ早期リターン*/
	if (!CanRotation())return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*使用する値の準備*/
	const VECTOR position  = this->model->GetPosition();//座標
		  VECTOR rotation  = { 0.0f,0.0f,0.0f };		 //回転率
		  this->moveTarget = player.GetPosition();
	
	/*プレイヤーから自分の座標までのベクトルを出す*/
	VECTOR positonToTargetVector = VSub(position, this->moveTarget);

	/*アークタンジェントを使って角度を求める*/
	rotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
	rotation.y = rotation.y * 180.0f / DX_PI_F;

	/*回転率を代入*/
	this->model->SetRotation(rotation);
}



/// <summary>
/// 攻撃を決める
/// </summary>
void Boss::DecideOfAttack() 
{
	/*スタミナが０以下だったらあったらリターンを返す*/
	if (this->stamina <= 0)return;

	/*攻撃をランダムで決める*/

}


const Collider Boss::GetCharacterCollider()
{
	return *this->collider[static_cast<int>(ColliderType::CHARACTER)];
}
const Collider Boss::GetAttackCollider()
{
	return *this->collider[static_cast<int>(ColliderType::ATTACK)];
}

/// <summary>
/// 突進攻撃
/// </summary>
void Boss::RushAttack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	
}
/// <summary>
/// ジャンプ攻撃
/// </summary>
void Boss::JumpAttack()
{
	///*シングルトンクラスのインスタンスの取得*/
	//auto& json = Singleton<JsonManager>::GetInstance();
	////auto& player = Singleton<PlayerManager>::GetInstance();

	///*攻撃前*/
	//int WAIT_TIME = json.GetJson(JsonManager::FileType::ENEMY)["WAIT_TIME_BEFORE_JUMP"];
	////目標待機時間に達していなかったら早期リターン
	//if (this->waitingCountBeforeAttack < WAIT_TIME)
	//{
	//	/*回転率の更新*/
	//	UpdateRotation();
	//	//カウントを増加
	//	this->waitingCountBeforeAttack++;
	//	//ジャンプ力の初期化
	//	this->jumpPower = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_POWER"];
	//	//早期リターン
	//	return;
	//}

	///*攻撃中*/
	//int ATTACK_TIME = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_TIME"];
	//float ATTACK_RANGE = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_RANGE"];

	////目標攻撃時間に達していたら落下させる
	//if (this->attackCount >= ATTACK_TIME)
	//{
	//	//座標の取得
	//	VECTOR position = this->model->GetPosition();
	//	//重力の取得
	//	const float GRAVITY = json.GetJson(JsonManager::FileType::ENEMY)["GRAVITY"];
	//	//移動ベクトルの更新
	//	VECTOR moveVector = VGet(0.0f, GRAVITY, 0.0f);
	//	//座標の更新
	//	position = VAdd(position, moveVector);

	//	/*地面についていたら待機状態にする*/
	//	const float HIT_SCALE = json.GetJson(JsonManager::FileType::ENEMY)["HIT_SCALE"];
	//	if (position.y <= HIT_SCALE)
	//	{
	//		position.y = HIT_SCALE;
	//		//「待機」状態に設定する
	//		this->state->ClearFlag(this->attackType);
	//		this->state->SetFlag(this->IDLE);
	//		this->isRest = true;
	//	}
	//	this->model->SetPosition(position);
	//	return;
	//}
	////ターゲット座標に近づいたらカウントを増加
	//VECTOR moveTarget = this->moveTarget;
	//moveTarget.y = 0.0f;
	//VECTOR position = this->model->GetPosition();
	//position.y = 0.0f;
	//float size = VSize(VSub(moveTarget, position));
	//if (size <= ATTACK_RANGE)
	//{
	//	this->attackCount++;
	//}
	//if (this->attackCount == 0)
	//{
	//	/*座標移動*/
	//	//速度の取得
	//	const float VELOCITY = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_VELOCITY"];
	//	//座標の取得
	//	VECTOR position = this->model->GetPosition();
	//	//ジャンプ
	//	const float JUMP_HEIGHT = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_HEIGHT"];
	//	moveTarget.y = JUMP_HEIGHT;
	//	this->moveVector = VNorm(VSub(moveTarget, this->model->GetPosition()));
	//	//座標の更新
	//	position = VAdd(position, VScale(this->moveVector, VELOCITY));
	//	this->model->SetPosition(position);
	//}
}

/// <summary>
/// 回転できるか
/// </summary>
/// <returns></returns>
const bool Boss::CanRotation()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK) || this->state->CheckFlag(this->REST))return false;
	return true;
}

const bool Boss::CanMove()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return false;
	if (this->state->CheckFlag(this->TAUNT))return false;
	return true;
}
/// <summary>
/// 攻撃できるか
/// </summary>
const bool Boss::CanAttack()const
{
	//if (this->state->CheckFlag(this->PUNCH_ATTACK))		 return false;
	//if (this->state->CheckFlag(this->KNOCKGROUND_ATTACK))return false;
	//if (this->state->CheckFlag(this->RUSH_ATTACK))		 return false;
	//if (this->state->CheckFlag(this->BREATH_ATTACK))	 return false;
	return true;
}
/// <summary>
/// 攻撃できるか
/// </summary>
const bool Boss::CanRest()const
{
	/*攻撃中かつアニメーションが終了している*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->model->GetIsChangeAnim())
	{
		//コンボカウントが0だったら
		if (this->attackComboCount <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
/// <summary>
/// 指定したフレームの計測
/// </summary>
bool Boss::FrameCount(const int _index, const int _maxFrame)
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
/// 項目ごとの関数の追加
/// </summary>
void Boss::AddItemFunction(const unsigned int _item, const FlagsState _update)
{
	FlagsStateSet add;
	add.update = _update;
	this->stateFunctionMap.emplace(_item, add);
}
/// <summary>
/// ダメージの取得
/// </summary>
const int Boss::GetDamage()const
{
	return this->damage;
}
const bool Boss::IsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}