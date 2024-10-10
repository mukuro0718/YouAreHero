#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "PlaneColliderData.h"
#include "LoadingAsset.h"
#include "BossMap.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossMap::BossMap()
	: collider(nullptr)
	, modelHandle(-1)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::FINALY_BOSS_STAGE));
	
	/*コライダーデータの作成*/
	this->collider = new PlaneColliderData(ColliderData::Priority::STATIC, GameObjectTag::GROUND);
	auto planeColiderData = dynamic_cast<PlaneColliderData*>(this->collider);
	planeColiderData->norm = Convert(json.GetJson(JsonManager::FileType::MAP)["MAP_NORM"]);
	planeColiderData->radius = json.GetJson(JsonManager::FileType::MAP)["MAP_SCALE"];
}

/// <summary>
/// デストラクタ
/// </summary>
BossMap::~BossMap()
{
}

void BossMap::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_POSITION"]);
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_SCALE"]);	
	VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_ROTATION"]);
	rotation.y = rotation.y * (DX_PI_F / 180.0f);
	/*物理挙動の初期化*/
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(SCALE);
}
/// <summary>
/// 更新
/// </summary>
void BossMap::Finalize()
{
	/*物理登録の解除*/
	DeleteMemberInstance(this->collider);
}
/// <summary>
/// 更新
/// </summary>
void BossMap::Update()
{
}
/// <summary>
/// 描画
/// </summary>
const void BossMap::Draw()const
{
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());
	MV1DrawModel(this->modelHandle);
}

