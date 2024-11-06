//==========================================
// @brief 攻撃用のスフィアColliderData
//==========================================
#pragma once
#include "GameObjectTag.h"

class ColliderData;
class AttackData;
class CharacterData;
class AttackSphereColliderData : public ColliderData
{
public:
	/*コンストラクタ*/
	AttackSphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data);

	void OnHit(const CharacterData& _data);//当たった時の処理

	/*メンバ変数*/
	AttackData* data;	//攻撃データ
	float		radius;	//半径
};
