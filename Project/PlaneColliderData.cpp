#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "PlaneColliderData.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlaneColliderData::PlaneColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag)
	: ColliderData	(ColliderData::Kind::PLANE, _priority, _tag)
	, norm			(VGet(0.0f, 0.0f, 0.0f))
	, radius		(0.0f)
{
	//処理なし
}