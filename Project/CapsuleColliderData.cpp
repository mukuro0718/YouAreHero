#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CapsuleColliderData.h"

CapsuleColliderData::CapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag)
	: ColliderData(ColliderData::Kind::CAPSULE, _priority, _tag)
	, radius(0.0f)
	, topPositon(VGet(0.0f, 0.0f, 0.0f))
{
	//èàóùÇ»Çµ
}