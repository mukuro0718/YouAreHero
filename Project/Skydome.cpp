#include <DxLib.h>
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "LoadingAsset.h"
#include "Skydome.h"

/// <summary>
/// コンストラクタ
/// </summary>
Skydome::Skydome()
	: model(-1)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->model = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::SKYDOME));
}

/// <summary>
/// デストラクタ
/// </summary>
Skydome::~Skydome()
{

}

/// <summary>
/// 初期化
/// </summary>
void Skydome::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_POSITION"]);
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_SCALE"]);
		  VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_ROTATION"]);
	rotation.y = rotation.y * (DX_PI_F / 180.0f);


	/*モデルのトランスフォームの設定*/
	this->position	= POSITION;
	this->rotation	= rotation;
	this->scale		= SCALE;
}

/// <summary>
/// 更新
/// </summary>
void Skydome::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*回転*/
	this->rotation.y += static_cast<float>(json.GetJson(JsonManager::FileType::MAP)["ADD_SKYDOME_ROTATE"]);
}

/// <summary>
/// 描画
/// </summary>
const void Skydome::Draw()const
{
	MV1SetPosition(this->model,this->position);
	MV1SetRotationXYZ(this->model,this->rotation);
	MV1SetScale(this->model,this->scale);
	MV1DrawModel(this->model);
}