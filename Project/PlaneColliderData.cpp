#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "PlaneColliderData.h"

PlaneColliderData::PlaneColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag)
	: ColliderData	(ColliderData::Kind::PLANE, _priority, _tag)
	, norm			(VGet(0.0f, 0.0f, 0.0f))
	, radius		(0.0f)
{
	//ˆ—‚È‚µ
}