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
	auto& json = Singleton<JsonManager>::GetInstance();

	this->model		= MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::SKYDOME));
	this->addRotate = static_cast<float>(json.GetJson(JsonManager::FileType::MAP)["ADD_SKYDOME_ROTATE"]);
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
	const VECTOR POSITION	= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_POSITION"]);
	const VECTOR SCALE		= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_SCALE"]);
		  VECTOR rotation	= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_ROTATION"]);
				 rotation.y = rotation.y * (DX_PI_F / 180.0f);

	/*モデルのトランスフォームの設定*/
	this->position	= POSITION;
	this->rotation	= rotation;
	this->scale		= SCALE;
	MV1SetPosition(this->model, this->position);
	MV1SetRotationXYZ(this->model, this->rotation);
	MV1SetScale(this->model, this->scale);
}

/// <summary>
/// 更新
/// </summary>
void Skydome::Update()
{
	/*回転*/
	this->rotation.y -= this->addRotate;
	MV1SetRotationXYZ(this->model,this->rotation);
}

/// <summary>
/// 描画
/// </summary>
const void Skydome::Draw()const
{
	MV1DrawModel(this->model);
}