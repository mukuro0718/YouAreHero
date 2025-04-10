#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "ModelColliderData.h"
#include "LoadingAsset.h"
#include "Dungeon.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dungeon::Dungeon()
	: collider(nullptr)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Dungeon::~Dungeon()
{
	/*物理登録の解除*/
	DeleteMemberInstance(this->collider);
}

/// <summary>
/// 描画
/// </summary>
const void Dungeon::Draw()const
{
	MV1DrawModel(this->modelHandle);
}

/// <summary>
/// モデルハンドルの取得
/// </summary>
const int Dungeon::GetModelHandle()const
{
	return this->modelHandle;
}

/// <summary>
/// 当たり判定フラグを下す
/// </summary>
void Dungeon::OffIsDoHitCheck()
{
	this->collider->isDoHitCheck = false;
}