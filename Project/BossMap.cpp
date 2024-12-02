#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "ModelColliderData.h"
#include "LoadingAsset.h"
#include "BossMap.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossMap::BossMap()
	: collider(nullptr)
{
	/*コライダーデータの作成*/
	this->collider = new ModelColliderData(ColliderData::Priority::STATIC, GameObjectTag::GROUND);
	
	/*モデルハンドルの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& modelColiderData = dynamic_cast<ModelColliderData&>(*this->collider);
	modelColiderData.modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::FINALY_BOSS_STAGE));
}

/// <summary>
/// デストラクタ
/// </summary>
BossMap::~BossMap()
{
}

/// <summary>
/// 初期化
/// </summary>
void BossMap::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/

	/*リジッドボディの初期化*/
	auto& json = Singleton<JsonManager>::GetInstance();
	VECTOR position = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_POSITION"]);
	VECTOR scale	= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_SCALE"]);
	VECTOR rotation = Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_ROTATION"]);
	rotation.y = rotation.y * (DX_PI_F / 180.0f);
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(position);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(scale);

	/*モデルの設定*/
	auto& modelColiderData = dynamic_cast<ModelColliderData&>(*this->collider);
	modelColiderData.frameIndex = json.GetJson(JsonManager::FileType::MAP)["COLL_FRAME_INDEX"];
	MV1SetPosition(modelColiderData.modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(modelColiderData.modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale(modelColiderData.modelHandle, this->collider->rigidbody.GetScale());
	MV1SetFrameVisible(modelColiderData.modelHandle, modelColiderData.frameIndex, FALSE);
	MV1SetupCollInfo(modelColiderData.modelHandle, modelColiderData.frameIndex, 16, 16, 16);
}

/// <summary>
/// 後処理
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
	auto& modelColiderData = dynamic_cast<ModelColliderData&>(*this->collider);
	MV1DrawModel(modelColiderData.modelHandle);
}

/// <summary>
/// モデルハンドルの取得
/// </summary>
const int BossMap::GetModelHandle()const
{ 
	auto& modelColiderData = dynamic_cast<ModelColliderData&>(*this->collider);
	return modelColiderData.modelHandle;
}