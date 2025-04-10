#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "ModelColliderData.h"
#include "LoadingAsset.h"
#include "Dungeon.h"
#include "BeastDungeon.h"

/// <summary>
/// コンストラクタ
/// </summary>
BeastDungeon::BeastDungeon()
	: Dungeon()
{
	/*コライダーデータの作成*/
	this->collider = new ModelColliderData(ColliderData::Priority::STATIC, GameObjectTag::GROUND);

	/*モデルハンドルの取得*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->collider->modelHandle	= MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::BEAST_DUNGEON));
	this->modelHandle			= MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::BEAST_DUNGEON));
	MV1SetupCollInfo (this->collider->modelHandle, this->collider->frameIndex, 16, 16, 16);
}

/// <summary>
/// デストラクタ
/// </summary>
BeastDungeon::~BeastDungeon()
{
#ifdef _DEBUG
	if (this->collider->isDoHitCheck)
	{
		printfDx("BeastDungeonは使用されています");
	}
#endif // _DEBUG
}

/// <summary>
/// 初期化
/// </summary>
void BeastDungeon::Initialize()
{
	/*リジッドボディの初期化*/
	auto& json = Singleton<JsonManager>::GetInstance();
	VECTOR position		= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["BEAST_DUNGEON_POSITION"]);
	VECTOR scale		= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["BEAST_DUNGEON_SCALE"]);
	VECTOR rotation		= Gori::Convert(json.GetJson(JsonManager::FileType::MAP)["BEAST_DUNGEON_ROTATION"]);
		   rotation.y	= rotation.y * (DX_PI_F / 180.0f);
	this->collider->rigidbody.Initialize(false);
	this->collider->rigidbody.SetPosition(position);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(scale);

	/*当たり判定用モデルの設定*/
	this->collider->frameIndex = -1;
	this->collider->isDoHitCheck = true;
	MV1SetPosition	 (this->collider->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->collider->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->collider->modelHandle, this->collider->rigidbody.GetScale());
	MV1RefreshCollInfo(this->collider->modelHandle, this->collider->frameIndex);
	//MV1SetupReferenceMesh(this->collider->modelHandle, this->collider->frameIndex, TRUE);
	//this->refPoly = MV1GetReferenceMesh(this->collider->modelHandle, this->collider->frameIndex, TRUE);

	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());
}

/// <summary>
/// 更新
/// </summary>
void BeastDungeon::Update()
{
#ifdef _DEBUG
	if (this->collider->isDoHitCheck)
	{
		printfDx("BeastDungeonは使用されています\n");
	}
#endif // _DEBUG
}

/// <summary>
/// 描画
/// </summary>
const void BeastDungeon::Draw()const
{
	if (this->collider->isDoHitCheck)
	{
		MV1DrawModel(this->modelHandle);
	}
//#ifdef _DEBUG
//	// ポリゴンの数だけ繰り返し
//	for (int i = 0; i < this->refPoly.PolygonNum; i++)
//	{
//		// ポリゴンを形成する三頂点を使用してワイヤーフレームを描画する
//		DrawLine3D(
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[0]].Position,
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[1]].Position,
//			GetColor(255, 255, 0));
//
//		DrawLine3D(
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[1]].Position,
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[2]].Position,
//			GetColor(255, 255, 0));
//
//		DrawLine3D(
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[2]].Position,
//			this->refPoly.Vertexs[this->refPoly.Polygons[i].VIndex[0]].Position,
//			GetColor(255, 255, 0));
//	}
//#endif // _DEBUG
}
