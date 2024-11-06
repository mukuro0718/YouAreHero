//==========================================
// @brief 球のColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class SphereColliderData : public ColliderData
{
public:
	/*コンストラクタ*/
	SphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);

	/*メンバ変数*/
	float radius;//半径
};
