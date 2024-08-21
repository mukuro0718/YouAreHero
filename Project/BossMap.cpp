#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "Model.h"
#include "LoadingAsset.h"
#include "BossMap.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossMap::BossMap()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::FINALY_BOSS_STAGE));
}

/// <summary>
/// デストラクタ
/// </summary>
BossMap::~BossMap()
{
	DeleteMemberInstance(this->model);
}

void BossMap::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_SCALE"]);	
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_ROTATION"]);
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::MAP)["FINALY_BOSS_MAP_POSITION"]);


	this->model->SetTransform(POSITION, ROTATION, SCALE);
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
	this->model->Draw();
}

