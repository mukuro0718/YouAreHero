//================================================
// @brief モデル用コライダーデータ
// モデルとの当たり判定を行うときに使う
//================================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class ModelColliderData : public ColliderData
{
public:
	ModelColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);

	int modelHandle;
	int frameIndex;
	bool isDoHitCheck;//当たり判定を行うか
};

