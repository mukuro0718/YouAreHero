#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Model.h"
#include "LoadingAsset.h"
#include "Shield.h"

/// <summary>
/// コンストラクタ
/// </summary>
Shield::Shield()
	: model(nullptr)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::SHIELD));
}

/// <summary>
/// デストラクタ
/// </summary>
Shield::~Shield()
{

}

/// <summary>
/// 初期化
/// </summary>
void Shield::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION = VGet(0.0f, 0.0f, 0.0f);
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::PLAYER)["SHIELD_ROTATION"]);
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::PLAYER)["SHIELD_SCALE"]);

	/*モデルのトランスフォームの設定*/
	this->model->SetTransform(POSITION, ROTATION, SCALE);
}

/// <summary>
/// 更新
/// </summary>
void Shield::Update(const VECTOR _targetPosition)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*jsonデータを各定数型に代入*/
	const VECTOR POSITION_OFFSET = Convert(json.GetJson(JsonManager::FileType::PLAYER)["SHIELD_POSITION_OFFSET"]);
	
	/*座標を出す*/
	const VECTOR POSITION = _targetPosition + POSITION_OFFSET;

	/*座標と回転率の設定*/
	this->model->SetPosition(POSITION);
}

/// <summary>
/// 描画
/// </summary>
const void Shield::Draw()const
{
	this->model->Draw();
}