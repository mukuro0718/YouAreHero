#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "ModelColliderData.h"

ModelColliderData::ModelColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag)
	: ColliderData	(ColliderData::Kind::MODEL, _priority, _tag)
	, modelHandle	(-1)
	, frameIndex	(0)
	, isDoHitCheck	(false)
{

}