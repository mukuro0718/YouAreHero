//==========================================
// @brief 攻撃用のカプセルColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class CharacterData;
class AttackData;
class AttackCapsuleColliderData : public ColliderData
{
public:
	/*コンストラクタ*/
	AttackCapsuleColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data);

	void OnHit(const CharacterData& _data);//当たった時の処理

	/*メンバ変数*/
	AttackData* data;		//攻撃データ
	VECTOR		topPositon;	//カプセル上座標
	float		radius;		//半径
};
