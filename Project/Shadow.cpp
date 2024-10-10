#include <DxLib.h>
#include <math.h>
#include "UseSTL.h"
#include "Shadow.h"
#include "LoadingAsset.h"

/// <summary>
/// コンストラクタ
/// </summary>
Shadow::Shadow()
	: imageHandle(0)
{
	auto& asset = LoadingAsset::GetInstance();
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::SHADOW);
}
/// <summary>
/// デストラクタ
/// </summary>
Shadow::~Shadow()
{
}
/// <summary>
/// 最終処理
/// </summary>
void Shadow::Final()
{
}
/// <summary>
/// 描画
/// </summary>
void Shadow::Draw(const int _stageModelHandle, const VECTOR _targetPos, const float _shadowHeight, const float _shadowSize)
{
	MV1_COLL_RESULT_POLY_DIM groundPoly;//プレイヤー直下の地面ポリゴン
	MV1_COLL_RESULT_POLY* spherePolyNum;//球直下のポリゴンの数
	VERTEX3D Vertex[3];//頂点データ
	VECTOR pushBackVector;//押し戻し量
	int stageModelHandle = _stageModelHandle;//ステージモデルハンドル
	VECTOR targetPos = _targetPos;//影をつけたい目標の座標
	float shadowHeight = _shadowHeight;//影の高さ
	float shadowSize = _shadowSize;//影のサイズ

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// プレイヤーの直下に存在する地面のポリゴンを取得
	groundPoly = MV1CollCheck_Capsule(_stageModelHandle, -1, _targetPos, VAdd(_targetPos, VGet(0.0f, -shadowHeight, 0.0f)), shadowSize);

	// 頂点データで変化が無い部分をセット
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	spherePolyNum = groundPoly.Dim;
	for (int i = 0; i < groundPoly.HitNum; i++, spherePolyNum++)
	{
		// ポリゴンの座標は地面ポリゴンの座標
		Vertex[0].pos = spherePolyNum->Position[0];
		Vertex[1].pos = spherePolyNum->Position[1];
		Vertex[2].pos = spherePolyNum->Position[2];

		// ちょっと持ち上げて重ならないようにする
		pushBackVector = VScale(spherePolyNum->Normal, 0.5f);
		Vertex[0].pos = VAdd(Vertex[0].pos, pushBackVector);
		Vertex[1].pos = VAdd(Vertex[1].pos, pushBackVector);
		Vertex[2].pos = VAdd(Vertex[2].pos, pushBackVector);

		// ポリゴンの不透明度を設定する
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if (spherePolyNum->Position[0].y > targetPos.y - shadowHeight)
			Vertex[0].dif.a = 128 * (1.0f - fabs(spherePolyNum->Position[0].y - targetPos.y) / shadowHeight);

		if (spherePolyNum->Position[1].y > targetPos.y - shadowHeight)
			Vertex[1].dif.a = 128 * (1.0f - fabs(spherePolyNum->Position[1].y - targetPos.y) / shadowHeight);

		if (spherePolyNum->Position[2].y > targetPos.y - shadowHeight)
			Vertex[2].dif.a = 128 * (1.0f - fabs(spherePolyNum->Position[2].y - targetPos.y) / shadowHeight);

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		Vertex[0].u = (spherePolyNum->Position[0].x - targetPos.x) / (shadowSize * 2.0f) + 0.5f;
		Vertex[0].v = (spherePolyNum->Position[0].z - targetPos.z) / (shadowSize * 2.0f) + 0.5f;
		Vertex[1].u = (spherePolyNum->Position[1].x - targetPos.x) / (shadowSize * 2.0f) + 0.5f;
		Vertex[1].v = (spherePolyNum->Position[1].z - targetPos.z) / (shadowSize * 2.0f) + 0.5f;
		Vertex[2].u = (spherePolyNum->Position[2].x - targetPos.x) / (shadowSize * 2.0f) + 0.5f;
		Vertex[2].v = (spherePolyNum->Position[2].z - targetPos.z) / (shadowSize * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		DrawPolygon3D(Vertex, 1, this->imageHandle, TRUE);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(groundPoly);

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);

	//const int VERTEX_NUM = 4;
	//const unsigned short VERTEX_INDEX[6] = { 0,1,2,0,2,3 };
	//VERTEX3D vertex[VERTEX_NUM];//頂点データ
	//VECTOR targetPos = _targetPos;//影をつけたい目標の座標
	//float stageHeight = _stageHeight;//影の高さ
	//float shadowSize = _shadowSize;//影のサイズ


	//// ポリゴンの座標は地面ポリゴンの座標
	//vertex[0].pos = VGet(-50.0f - shadowSize, stageHeight, 0.0f + shadowSize);
	//vertex[1].pos = VGet(-50.0f + shadowSize, stageHeight, 0.0f + shadowSize);
	//vertex[2].pos = VGet(-50.0f + shadowSize, stageHeight, 0.0f - shadowSize);
	//vertex[3].pos = VGet(-50.0f - shadowSize, stageHeight, 0.0f - shadowSize);
	//for (int i = 0; i < 4; i++)
	//{
	//	vertex[i].norm = VGet(0.0f, 1.0f, 0.0f);
	//	vertex[i].dif = GetColorU8(255, 255, 255, 255);
	//	vertex[i].spc = GetColorU8(255, 255, 255, 255);
	//	vertex[i].su = 0.0f;
	//	vertex[i].sv = 0.0f;
	//}
	//vertex[0].u = 0.0f;
	//vertex[0].v = 0.0f;
	//vertex[1].u = 1.0f;
	//vertex[1].v = 0.0f;
	//vertex[2].u = 1.0f;
	//vertex[2].v = 1.0f;
	//vertex[3].u = 0.0f;
	//vertex[3].v = 1.0f;
	//// 影ポリゴンを描画
	//DrawPolygonIndexed3D(vertex, VERTEX_NUM, VERTEX_INDEX, 2, this->imageHandle, TRUE);
}