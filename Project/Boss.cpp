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
	, collider	(nullptr)
	, moveVector{ 0.0f, 0.0f, 0.0f }
	, moveTarget{ 0.0f, 0.0f, 0.0f }
	, velocity	(0.0f)
	, waitingCountBeforeAttack(0)
	, restCount(0)
	, attackCount(0)
	, isRest(false)
	, attackType(0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->model		= new Model(asset.GetModel(LoadingAsset::ModelType::ENEMY));
	this->state		= new BitFlag();
	this->collider	= new Collider();

	/*攻撃関数の設定*/
	//auto rushAttack = [this]() {this->RushAttack(); };
	//this->attackFunctionMap.emplace(this->RUSH, rushAttack);

	/*攻撃種類の設定*/
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::RUSH), this->RUSH);
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::JUMP), this->JUMP);
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::LASER), this->LASER);
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::SPIN), this->SPIN);
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::INHIBITION), this->INHIBITION);

	this->state->SetFlag(this->IDLE);
}

/// <summary>
/// デストラクタ
/// </summary>
Boss::~Boss()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	DeleteMemberInstance(this->collider);
	this->attackFunctionMap.clear();
}

void Boss::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //拡大率

	this->velocity = 0.0f;
	this->waitingCountBeforeAttack = 0;
	this->restCount = 0;
	this->attackCount = 0;
	this->isRest = false;
	this->attackType = 0;

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

	/*攻撃の設定*/
	SetState();

	/*コライダーの設定*/
	const VECTOR MAX_OFFSET = Convert(json.GetJson(JsonManager::FileType::ENEMY)["MAX_HIT_POSITION_OFFSET"]);
	const VECTOR MIN_OFFSET = Convert(json.GetJson(JsonManager::FileType::ENEMY)["MIN_HIT_POSITION_OFFSET"]);
	const VECTOR POSITION = this->model->GetPosition();
	this->collider->SetOBB(POSITION + MAX_OFFSET, POSITION + MIN_OFFSET,this->model->GetPosition(), this->model->GetRotation());
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
void Boss::SetState()
{
	/*休憩していたら早期リターン*/
	if (IsRest())return;

	/*移動*/
	Move();

	/*攻撃処理*/
	//Attack();
}
bool Boss::IsRest()
{
	/*休憩していなかったら早期リターン*/
	if (!this->isRest)return false;


	/*休憩中だったら*/
	//カウントを増やす
	this->restCount++;

	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	const float REST_COUNT = json.GetJson(JsonManager::FileType::ENEMY)["REST_COUNT"];

	/*カウントが目標カウントを超えていたら早期リターン*/
	if (this->restCount >= REST_COUNT)
	{
		this->isRest = false;
		this->restCount = 0;
		return false;
	}

	return true;
}

/// <summary>
/// 移動
/// </summary>
void Boss::Move()
{
	/*移動できなかったら早期リターン*/
	if (!CanMove())return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*プレイヤーからの距離が定数以上だったら追跡*/
	const float CHASE_RANGE = json.GetJson(JsonManager::FileType::ENEMY)["CHASE_RANGE"];
	const 
}

/// <summary>
/// 描画
/// </summary>
const void Boss::Draw()const
{
	VECTOR position = this->model->GetPosition();
	VECTOR rotation = this->model->GetRotation();
	printfDx("Boss_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	printfDx("Boss_VELOCITY:%f\n", this->velocity);
	this->model->Draw();
	this->collider->DrawHitBox();
}

/// <summary>
/// 速度の更新
/// </summary>
void Boss::UpdateMoveVector()
{
	VECTOR rotation = this->model->GetRotation();

	/*回転率をもとに移動ベクトルを出す*/
	this->moveVector = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));

	/*移動ベクトルに速度を加算*/
	this->moveVector = VNorm(this->moveVector);
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
	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*使用する値の準備*/
	const VECTOR position		= this->model->GetPosition();//座標
		  VECTOR rotation		= { 0.0f,0.0f,0.0f };		 //回転率
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

/// <summary>
/// 移動できるか
/// </summary>
const bool Boss::CanMove()const
{
	if (this->isRest)						 return false;//休憩中か
	if (this->state->CheckFlag(this->TAUNT)) return false;//咆哮中か
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

const Collider Boss::GetCollider()
{
	return *this->collider;
}

/// <summary>
/// 突進攻撃
/// </summary>
void Boss::RushAttack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*攻撃前*/
	int WAIT_TIME = json.GetJson(JsonManager::FileType::ENEMY)["WAIT_TIME_BEFORE_RUSH"];
	//目標待機時間に達していなかったら早期リターン
	if (this->waitingCountBeforeAttack < WAIT_TIME)
	{
		/*回転率の更新*/
		UpdateRotation();
		//移動ベクトルの設定
		UpdateMoveVector();
		//カウントを増加
		this->waitingCountBeforeAttack++;
		//早期リターン
		return;
	}

	/*攻撃中*/
	int ATTACK_TIME = json.GetJson(JsonManager::FileType::ENEMY)["RUSH_TIME"];
	//カウントを増加
	this->attackCount++;
	//目標攻撃時間に達していたら「待機」状態に変更して早期リターン
	if (this->attackCount >= ATTACK_TIME)
	{
		//「待機」状態に設定する
		this->state->ClearFlag(this->attackType);
		this->state->SetFlag(this->IDLE);
		this->isRest = true;
		return;
	}
	const float VELOCITY = json.GetJson(JsonManager::FileType::ENEMY)["RUSH_VELOCITY"];
	VECTOR position = this->model->GetPosition();

	position = VAdd(position, VScale(this->moveVector, VELOCITY));
	this->model->SetPosition(position);
}
/// <summary>
/// ジャンプ攻撃
/// </summary>
void Boss::JumpAttack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	//auto& player = Singleton<PlayerManager>::GetInstance();

	/*攻撃前*/
	int WAIT_TIME = json.GetJson(JsonManager::FileType::ENEMY)["WAIT_TIME_BEFORE_JUMP"];
	//目標待機時間に達していなかったら早期リターン
	if (this->waitingCountBeforeAttack < WAIT_TIME)
	{
		/*回転率の更新*/
		UpdateRotation();
		//カウントを増加
		this->waitingCountBeforeAttack++;
		//ジャンプ力の初期化
		this->jumpPower = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_POWER"];
		//早期リターン
		return;
	}

	/*攻撃中*/
	int ATTACK_TIME = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_TIME"];
	float ATTACK_RANGE = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_RANGE"];

	//目標攻撃時間に達していたら落下させる
	if (this->attackCount >= ATTACK_TIME)
	{
		//座標の取得
		VECTOR position = this->model->GetPosition();
		//重力の取得
		const float GRAVITY = json.GetJson(JsonManager::FileType::ENEMY)["GRAVITY"];
		//移動ベクトルの更新
		VECTOR moveVector = VGet(0.0f, GRAVITY, 0.0f);
		//座標の更新
		position = VAdd(position, moveVector);

		/*地面についていたら待機状態にする*/
		const float HIT_SCALE = json.GetJson(JsonManager::FileType::ENEMY)["HIT_SCALE"];
		if (position.y <= HIT_SCALE)
		{
			position.y = HIT_SCALE;
			//「待機」状態に設定する
			this->state->ClearFlag(this->attackType);
			this->state->SetFlag(this->IDLE);
			this->isRest = true;
		}
		this->model->SetPosition(position);
		return;
	}
	//ターゲット座標に近づいたらカウントを増加
	VECTOR moveTarget = this->moveTarget;
	moveTarget.y = 0.0f;
	VECTOR position = this->model->GetPosition();
	position.y = 0.0f;
	float size = VSize(VSub(moveTarget, position));
	if (size <= ATTACK_RANGE)
	{
		this->attackCount++;
	}
	if (this->attackCount == 0)
	{
		/*座標移動*/
		//速度の取得
		const float VELOCITY = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_VELOCITY"];
		//座標の取得
		VECTOR position = this->model->GetPosition();
		//ジャンプ
		const float JUMP_HEIGHT = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_HEIGHT"];
		moveTarget.y = JUMP_HEIGHT;
		this->moveVector = VNorm(VSub(moveTarget, this->model->GetPosition()));
		//座標の更新
		position = VAdd(position, VScale(this->moveVector, VELOCITY));
		this->model->SetPosition(position);
	}
}