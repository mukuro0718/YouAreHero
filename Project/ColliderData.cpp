#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CollisionManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
ColliderData::ColliderData(const Kind _kind, const Priority _priority, const GameObjectTag _tag)
	: kind			(_kind)
	, isHit			(false)
	, nextPositon	(VGet(0.0f, 0.0f, 0.0f))
	, tag			(_tag)
	, priority		(_priority)
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& manager = Singleton<CollisionManager>::GetInstance();
	
	/*マネージャーに登録*/
	manager.Entry(*this);
}

/// <summary>
/// デストラクタ
/// </summary>
ColliderData::~ColliderData()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& manager = Singleton<CollisionManager>::GetInstance();

	/*マネージャーから削除*/
	manager.Exit(*this);
}