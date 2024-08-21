#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Model.h"
#include "LoadingAsset.h"
#include "Skydome.h"

/// <summary>
/// コンストラクタ
/// </summary>
Skydome::Skydome()
	: model(nullptr)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::SKYDOME));
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
	const VECTOR addPositionAndPosition = position + position;


	/*モデルのトランスフォームの設定*/
	this->model->SetTransform(position, rotation, scale);
}

/// <summary>
/// 描画
/// </summary>
const void Skydome::Draw()const
{
	this->model->Draw();
}