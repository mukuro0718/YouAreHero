//========================================================
// @brief 平面(今回は円形にするので半径も持つ)のColliderData
//========================================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class PlaneColliderData : public ColliderData
{
public:
	/*コンストラクタ*/
	PlaneColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag);

	/*メンバ変数*/
	VECTOR norm;	//法線
	float  radius;	//半径
};
