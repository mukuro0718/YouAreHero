#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "LoadingAsset.h"
#include "BossMap.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossMap::BossMap()
	: Collidable(Collidable::Priority::STATIC, GameObjectTag::GROUND, GoriLib::ColliderData::Kind::PLANE, false)
	, modelHandle(-1)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::FINALY_BOSS_STAGE));
	
	/*コライダーデータの作成*/
	auto planeColiderData = dynamic_cast<GoriLib::ColliderDataPlane*>(this->colliderData);
	planeColiderData->norm = Convert(json.GetJson(JsonManager::FileType::MAP)["MAP_NORM"]);
	planeColiderData->radius = json.GetJson(JsonManager::FileType::MAP)["MAP_SCALE"];
}

/// <summary>
/// デストラクタ
/// </summary>
BossMap::~BossMap()
{
}

void BossMap::Initialize(GoriLib::Physics* _physics)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_POSITION"]);
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_ROTATION"]);
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_SCALE"]);	

	/*コライダーの初期化*/
	Collidable::Initialize(_physics);

	/*物理挙動の初期化*/
	this->rigidbody.Initialize(false);
	this->rigidbody.SetPosition(POSITION);
	this->rigidbody.SetRotation(ROTATION);
	this->rigidbody.SetScale(SCALE);
}
/// <summary>
/// 更新
/// </summary>
void BossMap::Finalize(GoriLib::Physics* _physics)
{
	/*物理登録の解除*/
	Collidable::Finalize(_physics);
}
/// <summary>
/// 更新
/// </summary>
void BossMap::Update(GoriLib::Physics* _physics)
{
}
void BossMap::OnCollide(const Collidable& _colider)
{
	std::string message = "地面が";
	if (_colider.GetTag() == GameObjectTag::BOSS)
	{
		message += "ボス";
	}
	else if (_colider.GetTag() == GameObjectTag::GROUND)
	{
		message += "プレイヤー";
	}

	message += "と当たった\n";
	printfDx(message.c_str());
}
/// <summary>
/// 描画
/// </summary>
const void BossMap::Draw()const
{
	MV1SetPosition(this->modelHandle, this->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->rigidbody.GetRotation());
	MV1SetScale(this->modelHandle, this->rigidbody.GetScale());
	MV1DrawModel(this->modelHandle);
}

