#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "BeastBehaviorTreeHeader.h"
#include "Beast.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "Debug.h"
#include "HitStopManager.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast::Beast()
	: moveTarget			(Gori::ORIGIN)
	, animationPlayTime		(0.0f)
	, nowAnimation			(0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::BEAST));
	

	/*アニメーションの設定*/
	int				animationHandle	  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_HANDLE"];
	vector<int>		animationIndex	  = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_INDEX"];
			  this->nowAnimation	  = static_cast<int>(AnimationType::IDLE);
			  this->animationPlayTime = json.GetJson(JsonManager::FileType::BEAST)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//アニメーションの追加
	for (int i = 0; i < animationIndex.size(); i++)
	{
		this->animation->Add(animationHandle, animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

	/*コライダーデータの作成*/
	this->maxHp = json.GetJson(JsonManager::FileType::BEAST)["HP"];
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData());
	this->maxPartsColliderNum = json.GetJson(JsonManager::FileType::BEAST)["COLLIDER_NUM"];
	this->frameIndexUsePartsColider = json.GetJson(JsonManager::FileType::BEAST)["FRAME_INDEX_USE_PARTS_COLLIDER"];
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->partsCollider.emplace_back(new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData()));
		this->partsCollider[i]->data->hp			= this->maxHp;
		this->partsCollider[i]->radius				= json.GetJson(JsonManager::FileType::BEAST)["PARTS_COLL_RADIUS"][i];
		this->partsCollider[i]->isUseCollWithChara	= true;
		this->partsCollider[i]->isUseCollWithGround = false;
		this->partsCollider[i]->isSetTopPosition	= true;
		this->partsCollider[i]->rigidbody.Initialize(false);
		this->prevPartsHp.emplace_back(this->maxHp);
		this->pos1.emplace_back(Gori::ORIGIN);
		this->pos2.emplace_back(Gori::ORIGIN);
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Beast::~Beast()
{
	Finalize();
}

void Beast::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*変数の初期化*/
	this->isAlive						= true;
	this->isGround						= true;
	this->isDraw						= true;
	this->speed							= 0.0f;
	this->animationPlayTime				= 0.0f;
	this->entryInterval					= 0;
	this->moveTarget					= Gori::ORIGIN;
	this->nowAnimation					= static_cast<int>(AnimationType::ROAR);
	float height						= json.GetJson(JsonManager::FileType::BEAST)["HIT_HEIGHT"];
	this->collider->topPositon			= VGet(0.0f, height, 0.0f);
	this->collider->radius				= json.GetJson(JsonManager::FileType::BEAST)["HIT_RADIUS"];
	this->collider->isUseCollWithChara	= false;
	this->collider->isUseCollWithGround = true;
	this->collider->data->hp			= this->maxHp;
	this->collider->data->isHit			= false;
	
	/*物理挙動の初期化*/
	//jsonデータを定数に代入
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_ROTATION"]);//回転率
	const VECTOR DIRECTION = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_DIRECTION"]);//回転率
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_SCALE"]);	 //拡大率
	//初期化
	this->collider->rigidbody.Initialize(true);
	this->collider->isUseCollWithChara = false;
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetVelocity(DIRECTION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		this->prevPartsHp[i] = this->maxHp;
		
	}

	/*アニメーションのアタッチ*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// 後処理
/// </summary>
void Beast::Finalize()
{
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		DeleteMemberInstance(this->partsCollider[i]);
	}
	this->partsCollider.clear();
}

/// <summary>
/// 更新
/// </summary>
void Beast::Update()
{
	/*コライダーの更新*/
	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		//座標の設定
		this->pos1[i] = MV1GetFramePosition(this->modelHandle, this->frameIndexUsePartsColider[i][0]);
		this->pos2[i] = MV1GetFramePosition(this->modelHandle, this->frameIndexUsePartsColider[i][1]);
		this->partsCollider[i]->rigidbody.SetPosition(this->pos1[i]);
		this->partsCollider[i]->topPositon = this->pos2[i];
		//ダメージの計算
		if (this->partsCollider[i]->data->isHit)
		{
			this->partsCollider[i]->data->isHit = false;
			float damage = this->prevPartsHp[i] - this->partsCollider[i]->data->hp;
			this->prevPartsHp[i] = this->partsCollider[i]->data->hp;
			this->collider->data->hp -= damage;
		}
	}

	/*ビヘイビアツリーの更新*/
	auto& tree = Singleton<BeastBehaviorTree>::GetInstance();
	tree.Update();
}

/// <summary>
/// アニメーションの再生
/// </summary>
void Beast::PlayAnimation()
{
	//アニメーションの再生
	if (this->isAlive)
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		this->animation->Play(&this->modelHandle, this->nowAnimation, this->animationPlayTime);
		this->collider->rigidbody.SetPosition(position);
	}
}

/// <summary>
/// 描画
/// </summary>
const void Beast::DrawCharacterInfo()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& debug = Singleton<Debug>::GetInstance();
	auto& tree = Singleton<BeastBehaviorTree>::GetInstance();
	tree.Draw();
	
	if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
	{
		//VECTOR position = this->collider->rigidbody.GetPosition();
		//VECTOR rotation = this->collider->rigidbody.GetRotation();
		//printfDx("Beast_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
		//printfDx("Beast_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	}

	for (int i = 0; i < this->maxPartsColliderNum; i++)
	{
		VECTOR pos1 = this->pos1[i];
		VECTOR pos2 = this->pos2[i];
		DrawCapsule3D(pos1, pos2, this->partsCollider[i]->radius, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), FALSE);
		VECTOR underPos = this->partsCollider[i]->rigidbody.GetPosition();
		VECTOR topPos = this->partsCollider[i]->topPositon;
		DrawCapsule3D(underPos, topPos, this->partsCollider[i]->radius, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);
	}

	if (this->isDraw)
	{
		/*かげの描画*/
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
}

const bool Beast::GetIsAttack()const
{
	return false;
}

/// <summary>
/// アニメーション再生時間の取得
/// </summary>
const float Beast::GetAnimationPlayTime()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	return json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

/// <summary>
/// 回転率の設定
/// </summary>
void Beast::SetRotation(const VECTOR _rotation)
{
	this->collider->rigidbody.SetRotation(_rotation);
}

/// <summary>
/// 移動ベクトルの設定
/// </summary>
void Beast::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}