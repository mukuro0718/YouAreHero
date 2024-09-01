#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "Animation.h"
#include "Model.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "Boss.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "Vector4.h"
#include "BossAttackManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Boss::Boss()
	: Collidable(Collidable::Priority::HIGH,GameObjectTag::BOSS,GoriLib::ColliderData::Kind::CAPSULE,false)
	, state		 (nullptr)
	, animation  (nullptr)
	, moveVector { 0.0f, 0.0f, 0.0f }
	, moveTarget { 0.0f, 0.0f, 0.0f }
	, speed		 (0.0f)
	, modelHandle(-1)
	, isGround	 (false)
	, isHitAttack(false)
	, isDraw	 (true)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->animation	= new Animation();
	this->state		= new BitFlag();
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::ENEMY));

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
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(MV1LoadModel(animationHandle[i].c_str()), animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

	/*関数マップの設定*/
	auto idleSet	  = [this]() {Idle(); };
	auto roarSet	  = [this]() {Taunt(); };
	auto moveSet	  = [this]() {Move(); };
	auto deathSet	  = [this]() {Death(); };
	auto restSet	  = [this]() {Rest(); };
	this->AddItemFunction(this->IDLE					, idleSet);
	this->AddItemFunction(this->ROAR, roarSet);
	this->AddItemFunction(this->WALK, moveSet);
	this->AddItemFunction(this->REST					, restSet);
	this->AddItemFunction(this->DYING, deathSet);
	/*アニメーションマップの設定*/
	this->stateAnimationMap.emplace(this->DYING, static_cast<int>(AnimationType::DYING));
	this->stateAnimationMap.emplace(this->IDLE, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->ROAR, static_cast<int>(AnimationType::ROAR));
	this->stateAnimationMap.emplace(this->WALK, static_cast<int>(AnimationType::WALK));
	this->stateAnimationMap.emplace(this->REST, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->SLASH, static_cast<int>(AnimationType::SLASH));
	this->stateAnimationMap.emplace(this->ROTATE_PUNCH, static_cast<int>(AnimationType::ROTATE_PUNCH));
	this->stateAnimationMap.emplace(this->JUMP_ATTACK, static_cast<int>(AnimationType::JUMP_ATTACK));
	this->stateAnimationMap.emplace(this->HURRICANE_KICK, static_cast<int>(AnimationType::HURRICANE_KICK));
	this->stateAnimationMap.emplace(this->GRAND_SLAM, static_cast<int>(AnimationType::GRAND_SLAM));
	this->stateAnimationMap.emplace(this->FLAME_MAGIC, static_cast<int>(AnimationType::FLAME_MAGIC));
	this->stateAnimationMap.emplace(this->LIGHTNING, static_cast<int>(AnimationType::LIGHTNING));
	this->stateAnimationMap.emplace(this->CONTINUOUS_SLASH, static_cast<int>(AnimationType::CONTINUOUS_SLASH));
	this->stateAnimationMap.emplace(this->DARK_FIELD, static_cast<int>(AnimationType::DARK_FIELD));
	this->stateAnimationMap.emplace(this->METEO, static_cast<int>(AnimationType::METEO));


	/*コライダーデータの作成*/
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	capsuleColiderData->radius = json.GetJson(JsonManager::FileType::PLAYER)["HIT_RADIUS"];
	capsuleColiderData->height = json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];
	capsuleColiderData->isCutDamage = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Boss::~Boss()
{
	DeleteMemberInstance(this->animation);
	DeleteMemberInstance(this->state);
	this->stateFunctionMap.clear();
	this->stateAnimationMap.clear();
	this->frameCount.clear();
	this->isCount.clear();
}

void Boss::Initialize(GoriLib::Physics* _physics)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //拡大率

	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	this->speed = 0.0f;
	capsuleColiderData->hp = 1000;

	/*コライダーの初期化*/
	Collidable::Initialize(_physics);

	/*物理挙動の初期化*/
	this->rigidbody.Initialize(true);
	this->rigidbody.SetPosition(POSITION);
	this->rigidbody.SetRotation(ROTATION);
	this->rigidbody.SetScale(SCALE);
	this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];

	this->state->SetFlag(this->IDLE);
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// 後処理
/// </summary>
void Boss::Finalize(GoriLib::Physics* _physics)
{
	/*物理登録の解除*/
	Collidable::Finalize(_physics);
}

/// <summary>
/// 更新
/// </summary>
void Boss::Update(GoriLib::Physics* _physics)
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

	Move();

	/*状態の切り替え*/
	ChangeState();

	/*アニメーションの更新*/
	this->nowAnimation = this->stateAnimationMap[this->state->GetFlag()];
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	FrameCount(static_cast<int>(FrameCountType::SLASH), json.GetJson(JsonManager::FileType::ENEMY)["SLASH_SLOW_FRAME_COUNT"]);
	FrameCount(static_cast<int>(FrameCountType::ROTATE_PUNCH), json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_PUNCH_SLOW_FRAME_COUNT"]);
	if (this->isCount[static_cast<int>(FrameCountType::SLASH)] || this->isCount[static_cast<int>(FrameCountType::ROTATE_PUNCH)])
	{
		this->animationPlayTime *= 0.1f;
	}

	VECTOR position = this->rigidbody.GetPosition();
	this->animation->Play(&this->modelHandle, position, this->nowAnimation, this->animationPlayTime);
	this->rigidbody.SetPosition(position);
}

/// <summary>
/// 咆哮
/// </summary>
void Boss::Taunt()
{
	/*咆哮中にアニメーションが終了していたらフラグを下す*/
	if (this->state->CheckFlag(this->ROAR) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->ROAR);
	}
}
/// <summary>
/// 移動
/// </summary>
void Boss::Move()
{
	/*移動ベクトルの初期化*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*回転率を出す*/
	UpdateRotation();

	/*移動速度の更新*/
	UpdateVelocity();

	/*移動ベクトルを出す*/
	UpdateMoveVector();
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
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& attack = Singleton<BossAttackManager>::GetInstance();
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);

	if (capsuleColiderData->GetHP() < 0)
	{
		this->state->SetFlag(this->DYING);
		if (this->animation->GetIsChangeAnim())
		{
			this->isDraw = false;
		}
	}

	/*デスしていなければ*/
	if (this->state->CheckFlag(this->DYING))return;

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
	const float TARGET_DISTANCE = VSize(VSub(player.GetPosition(), this->rigidbody.GetPosition()));//プレイヤーとの距離を求める
	const float MOVE_DISTANCE = json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"];//目標との最大距離
	const float THROW_DISTANCE = json.GetJson(JsonManager::FileType::ENEMY)["THROW_DISTANCE"];//目標との最大距離
	const float MOVE_DISTANCE_2 = json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE_2"];//目標との最大距離
	
	this->state->ClearFlag(this->WALK | this->IDLE);

	if (TARGET_DISTANCE >= MOVE_DISTANCE_2)
	{
		this->state->SetFlag(this->WALK);
	}
	/*石を投げる*/
	else if (TARGET_DISTANCE >= THROW_DISTANCE)
	{
		int attackType = static_cast<int>(AttackType::JUMP_ATTACK);
		this->state->SetFlag(this->JUMP_ATTACK);
		this->attackNumber++;
		attack.OnIsStart(attackType);
		effect.OnIsBossThrowEffect();
		this->isCount[static_cast<int>(FrameCountType::JUMP_ATTACK)] = true;
	}
	/*もしプレイヤーとの距離が最大距離以上離れていたら追跡する*/
	else if (TARGET_DISTANCE >= MOVE_DISTANCE)
	{
		this->state->SetFlag(this->WALK);
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

		/*phase1*/
		//今向いている方向とプレイヤーへの咆哮のない席が一定以上だったら回転切りをする
		VECTOR toPlayer = VNorm(VSub(player.GetPosition(), this->rigidbody.GetPosition()));
		int type = GetRand(1);
		if (type == 0)
		{
			this->state->SetFlag(this->SLASH);
			attackType = static_cast<int>(AttackType::SLASH);
			this->attackNumber++;
			attack.OnIsStart(attackType);
			this->isCount[static_cast<int>(FrameCountType::SLASH)] = true;
			effect.OnIsBossPunchEffect();
		}
		else
		{
			this->state->SetFlag(this->ROTATE_PUNCH);
			attackType = static_cast<int>(AttackType::ROTATE_PUNCH);
			this->attackNumber++;
			attack.OnIsStart(attackType);
			this->isCount[static_cast<int>(FrameCountType::ROTATE_PUNCH)] = true;
			effect.OnIsBossSlashEffect();
		}


		/*phase2*/


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
		MV1SetPosition(this->modelHandle, this->rigidbody.GetPosition());
		MV1SetRotationXYZ(this->modelHandle, this->rigidbody.GetRotation());
		MV1SetScale(this->modelHandle, this->rigidbody.GetScale());
		MV1DrawModel(this->modelHandle);
	}
}

/// <summary>
/// 速度の更新
/// </summary>
void Boss::UpdateMoveVector()
{
	/*移動ベクトルを初期化する*/
	VECTOR direction = { 0.0f,0.0f,0.0f };
	VECTOR rotation = this->rigidbody.GetRotation();

	/*回転率をもとに移動ベクトルを出す*/
	direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));

	/*移動ベクトルを正規化*/
	direction = VNorm(direction);

	/*移動ベクトルの方向を代入*/
	this->direction = direction;

	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);

	this->rigidbody.SetVelocity(newVelocity);
}
/// <summary>
/// 速度の更新
/// </summary>
void Boss::UpdateVelocity()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->state->CheckFlag(this->WALK))
	{
		this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	}
	else if (this->state->CheckFlag(this->JUMP_ATTACK))
	{
		if (this->isCount[static_cast<int>(FrameCountType::JUMP_ATTACK)])
		{
			this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
			if (FrameCount(static_cast<int>(FrameCountType::JUMP_ATTACK), json.GetJson(JsonManager::FileType::ENEMY)["JUMP_MOVE_FRAME_COUNT"]))
			{
				this->speed = 0.0f;
			}
		}
		else
		{
			this->speed = 0.0f;
		}
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
const VECTOR Boss::GetPosition()const
{
	return this->rigidbody.GetPosition();
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
	const VECTOR position  = this->rigidbody.GetPosition();//座標
		  VECTOR rotation  = { 0.0f,0.0f,0.0f };		 //回転率
		  this->moveTarget = player.GetPosition();
	
	/*プレイヤーから自分の座標までのベクトルを出す*/
	VECTOR positonToTargetVector = VSub(position, this->moveTarget);

	/*アークタンジェントを使って角度を求める*/
	rotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
	//rotation.y = rotation.y * 180.0f / DX_PI_F;

	/*回転率を代入*/
	this->rigidbody.SetRotation(rotation);
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
/// 回転できるか
/// </summary>
/// <returns></returns>
const bool Boss::CanRotation()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK)/* || this->state->CheckFlag(this->REST)*/)return false;
	return true;
}

const bool Boss::CanMove()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return false;
	if (this->state->CheckFlag(this->ROAR))return false;
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
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
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
const bool Boss::IsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}
const VECTOR Boss::GetDirection()const
{
	return this->rigidbody.GetDirection();
}
const VECTOR Boss::GetRotation()const
{
	return this->rigidbody.GetRotation();
}
const VECTOR Boss::GetHeadPosition()const
{
	return MV1GetFramePosition(this->modelHandle, 7);
}
void Boss::OnCollide(const Collidable& _colider)
{
	std::string message = "ボスが";
	if (_colider.GetTag() == GameObjectTag::BOSS)
	{
		message += "プレイヤー";
	}
	else if (_colider.GetTag() == GameObjectTag::GROUND)
	{
		message += "地面";
	}

	message += "と当たった\n";
	printfDx(message.c_str());
}

const int Boss::GetHP()const
{ 
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	return capsuleColiderData->GetHP();
}
