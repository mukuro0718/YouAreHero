#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CollisionManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
ColliderData::ColliderData(const Kind _kind, const Priority _priority, const GameObjectTag _tag)
	: isHit(false)
	, tag(_tag)
	, priority(_priority)
	, kind(_kind)
	, nextPositon(VGet(0.0f,0.0f,0.0f))
{
	auto& manager = Singleton<CollisionManager>::GetInstance();
	manager.Entry(*this);
}

/// <summary>
/// デストラクタ
/// </summary>
ColliderData::~ColliderData()
{
	auto& manager = Singleton<CollisionManager>::GetInstance();
	manager.Exit(*this);
}