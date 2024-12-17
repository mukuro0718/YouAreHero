#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "BossData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"
#include "Enemy.h"
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
	CharacterData* data = new BossData();
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, data);

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
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);

	/*変数の初期化*/
	this->isAlive			= true;
	this->isGround			= true;
	this->isDraw			= true;
	this->speed				= 0.0f;
	this->animationPlayTime	= 0.0f;
	this->entryInterval		= 0;
	this->moveTarget		= Gori::ORIGIN;
	this->nowAnimation		= static_cast<int>(AnimationType::ROAR);
	float height			= json.GetJson(JsonManager::FileType::BEAST)["HIT_HEIGHT"];
	collider.topPositon		= VGet(0.0f, height, 0.0f);
	collider.radius			= json.GetJson(JsonManager::FileType::BEAST)["HIT_RADIUS"];
	collider.isUseCollWithGround = true;
	data.hp					= json.GetJson(JsonManager::FileType::BEAST)["HP"];
	data.isHit				= false;
	
	/*物理挙動の初期化*/
	//jsonデータを定数に代入
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::BEAST)["INIT_SCALE"]);	 //拡大率
	//初期化
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());


	/*アニメーションのアタッチ*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// 後処理
/// </summary>
void Beast::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void Beast::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
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
	
	if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
	{
		//VECTOR position = this->collider->rigidbody.GetPosition();
		//VECTOR rotation = this->collider->rigidbody.GetRotation();
		//printfDx("Beast_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
		//printfDx("Beast_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
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