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

void Skydome::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR position = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_POSITION"]);
	const VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_ROTATION"]);
	const VECTOR scale = Convert(json.GetJson(JsonManager::FileType::MAP)["SKYDOME_SCALE"]);


	/*モデルのトランスフォームの設定*/
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
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