//==========================================
// @brief カプセルのColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CapsuleColliderData : public ColliderData
{
public:
	/*コンストラクタ*/
	CapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);

	/*メンバ変数*/
	float	radius;		//半径
	VECTOR	topPositon;	//カプセル上座標
};
