#include <cassert>
#include <DxLib.h>
#include "DeleteInstance.h"
#include "GoriLib.h"
using namespace GoriLib;

/// <summary>
/// コンストラクタ
/// </summary>
Collidable::Collidable(Priority _priority, GameObjectTag _tag, ColliderData::Kind _colliderKind, bool _isTrigger)
	: priority(_priority)
	, tag(_tag)
	, colliderData(nullptr)
	, nextPosition(VGet(0.0f, 0.0f, 0.0f))
{
	CreateColliderData(_colliderKind, _isTrigger);
}

/// <summary>
/// デストラクタ
/// </summary>
Collidable::~Collidable()
{
	DeleteMemberInstance(this->colliderData);
}

/// <summary>
/// 初期化
/// </summary>
void Collidable::Initialize(GoriLib::Physics* _physics)
{
	_physics->Entry(this);
}

/// <summary>
/// 終了
/// </summary>
void Collidable::Finalize(GoriLib::Physics* _physics)
{
	_physics->Exit(this);
}

/// <summary>
/// 当たり判定を無視するタグの追加
/// </summary>
/// <param name="_tag"></param>
void Collidable::AddThroughTag(GameObjectTag _tag)
{
	bool found = (std::find(this->throughTags.begin(), this->throughTags.end(), this->tag) != this->throughTags.end());
	if (found)
	{
		assert(0 && "指定タグは既に追加されています。");
	}
	else
	{
		this->throughTags.emplace_back(this->tag);
	}
}

/// <summary>
/// 当たり判定を無視するタグの削除
/// </summary>
/// <param name="_tag"></param>
void Collidable::RemoveThroughTag(GameObjectTag _tag)
{
	bool found = (std::find(this->throughTags.begin(), this->throughTags.end(), this->tag) != this->throughTags.end());
	if (found)
	{
		assert(0 && "指定タグは存在しません。");
	}
	else
	{
		this->throughTags.remove(this->tag);
	}
}

/// <summary>
/// 当たり判定を無視する対象かどうか
/// </summary>
bool Collidable::IsThroughTarget(const Collidable* _target)const
{
	bool found = (std::find(this->throughTags.begin(), this->throughTags.end(), this->tag) != this->throughTags.end());
	return found;
}

/// <summary>
/// 当たり判定データの作成
/// </summary>
ColliderData* Collidable::CreateColliderData(ColliderData::Kind _kind, bool _isTrigger)
{
	if (this->colliderData != nullptr)
	{
		assert(0 && "colliderDataは既に作られています");
		return this->colliderData;
	}

	switch (_kind)
	{
	case GoriLib::ColliderData::Kind::SPHERE:
		this->colliderData = new ColliderDataSphere(_isTrigger);
		break;
	case GoriLib::ColliderData::Kind::CAPSULE:
		this->colliderData = new ColliderDataCapsule(_isTrigger);
		break;
	case GoriLib::ColliderData::Kind::PLANE:
		this->colliderData = new ColliderDataPlane(_isTrigger);
		break;
	//case GoriLib::ColliderData::Kind::CUBE:
	//	break;
	//default:
	//	break;
	}
	return this->colliderData;
}