#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "SphereColliderData.h"

/// <summary>
/// コンストラクタ
/// </summary>
SphereColliderData::SphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag)
	: ColliderData	(ColliderData::Kind::SPHERE, _priority, _tag)
	, radius		(0.0f)
{
	//処理なし
}