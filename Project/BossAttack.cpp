#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "GameObjectTag.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "BossAttackData.h"
#include "AttackCapsuleColliderData.h"
#include "BossAttack.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossAttack::BossAttack()
	: collider			(nullptr)
	, isStartHitCheck	(false)
	, frameCount		(0)
	, attackIndex		(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
BossAttack::~BossAttack()
{

}

/// <summary>
/// 後処理
/// </summary>
void BossAttack::Finalize()
{
}

///// <summary>
///// 描画
///// </summary>
//const void BossAttack::Draw()const
//{
//#if _DEBUG
//	if (this->isStartHitCheck)
//	{
//		DrawSphere3D(this->rigidbody.GetPosition(), this->GetRadius(), 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
//	}
//	VECTOR position = this->rigidbody.GetPosition();
//	printfDx("MAIN_1_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
//#endif // _DEBUG
//}

/// <summary>
/// 座標の取得
/// </summary>
const VECTOR BossAttack::GetPosition()const
{
	return this->collider->rigidbody.GetPosition();
}


const bool BossAttack::GetIsHitAttack()
{
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	return collider.data->isHitAttack;
}

void BossAttack::OffIsHitAttack()
{
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	collider.data->isHitAttack = false;
}

/// <summary>
/// std::vector<float>をVECTORに変換
/// </summary>
const VECTOR BossAttack::Convert(std::vector<float> _in)
{
	VECTOR out = VGet(0.0f, 0.0f, 0.0f);
	out.x = _in[0];
	out.y = _in[1];
	out.z = _in[2];
	return out;
}
