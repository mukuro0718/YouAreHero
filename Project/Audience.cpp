#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Audience.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "PlayerManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
Audience::Audience()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& json  = Singleton<JsonManager>::GetInstance();

	VERTEX3D base;
	base.pos = Gori::ORIGIN;
	base.norm = VGet(0.0, 0.0, 1.0);
	base.dif = GetColorU8(255, 255, 255, 255);
	base.spc = GetColorU8(0, 0, 0, 0);
	base.u = 0.0f;
	base.v = 0.0f;
	base.su = 0.0f;
	base.sv = 0.0f;
	for (int i = 0; i < this->VERTEX_NUM; i++)
	{
		this->vertex[i] = base;
		this->vertex[i].pos = Convert(json.GetJson(JsonManager::FileType::UI)["AUDIENCE_VERTEX_POSITION"][i]);
	}

	for (int i = 0; i < this->VERTEX_NUM; i++)
	{
		this->index[i] = json.GetJson(JsonManager::FileType::UI)["AUDIENCE_VERTEX_INDEX"][i];
	}
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::AUDIENCE);
}

/// <summary>
/// デストラクタ
/// </summary>
Audience::~Audience()
{

}

/// <summary>
/// 初期化
/// </summary>
void Audience::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*初期化*/
}

/// <summary>
/// 更新
/// </summary>
void Audience::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();


}

/// <summary>
/// 描画
/// </summary>
void Audience::Draw()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	//DrawPolygonIndexed3D(this->vertex, this->VERTEX_NUM, this->index, this->POLYGON_NUM, DX_NONE_GRAPH/*this->imageHandle*/, TRUE);
}

/// <summary>
/// 色取得
/// </summary>
int Audience::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}

/// <summary>
/// VECTORに変換
/// </summary>
VECTOR Audience::Convert(const std::vector<int> _in)
{
	VECTOR out;
	out.x = _in[0];
	out.y = _in[1];
	out.z = _in[2];

	return out;
}
