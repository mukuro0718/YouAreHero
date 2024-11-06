#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "UseMath.h"
#include "GameObjectTag.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "BossData.h"
#include "PlayerData.h"
#include "AttackData.h"
#include "BossAttackData.h"
#include "PlayerAttackData.h"
#include "ColliderData.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
#include "PlaneColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackSphereColliderData.h"
#include "CharacterColliderData.h"
#include "Singleton.h"
#include "CollisionManager.h"

/// <summary>
/// 衝突物の登録
/// </summary>
void CollisionManager::Entry(ColliderData& _data)
{
	/*登録されているか調べる*/
	bool found = (std::find(this->collidables.begin(), this->collidables.end(), &_data) != this->collidables.end());

	/*登録されていなかったら*/
	if (!found)
	{
		this->collidables.emplace_back(&_data);
	}

	/*登録されていたらエラーを吐く*/
	else
	{
		assert(0 && "指定のcollidableは登録済みです");
	}
}

/// <summary>
/// 衝突物の登録解除
/// </summary>
void CollisionManager::Exit(ColliderData& _data)
{
	/*登録されているか調べる*/
	bool found = (std::find(this->collidables.begin(), this->collidables.end(), &_data) != this->collidables.end());

	/*登録されていたら*/
	if (!found)
	{
		this->collidables.remove(&_data);
	}

	/*登録されていなかったらエラーを吐く*/
	else
	{
		assert(0 && "指定のcollidableは登録されていません");
	}

}

/// <summary>
/// 更新(登録オブジェクトの物理移動、衝突通知)
/// </summary>
void CollisionManager::Update()
{
	/*移動*/
	for (auto& item : this->collidables)
	{
		//ポジションに移動力を足す
		auto position = item->rigidbody.GetPosition();
		auto velocity = item->rigidbody.GetVelocity();
		//重力を利用する設定なら、重力を追加する
		if (item->rigidbody.UseGravity())
		{
			velocity = VAdd(velocity, VGet(0.0f, this->GRAVITY, 0));
			//最大重力加速度より大きかったらクランプ
			if (velocity.y < this->MAX_GRAVITY_ACCEL)
			{
				velocity = VGet(velocity.x, this->MAX_GRAVITY_ACCEL, velocity.z);
			}
		}
		//次の座標とVelocityを出す
		auto nextPosition = VAdd(position, velocity);
		item->rigidbody.SetVelocity(velocity);
		//もともとの情報、予定情報をデバッグ表示
#if _DEBUG
		auto kind = item->GetKind();
		if (kind == ColliderData::Kind::ATTACK_SPHERE)
		{
			//AttackSphereColliderData& sphereData = dynamic_cast<AttackSphereColliderData&>(*item);
			//float radius = sphereData.radius;
			//DrawSphere3D(position, radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			//DrawSphere3D(nextPosition, radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
		else if (kind == ColliderData::Kind::ATTACK_CAPSULE)
		{
			//AttackCapsuleColliderData& capsuleData = dynamic_cast<AttackCapsuleColliderData&>(*item);
			//float radius = capsuleData.radius;
			//VECTOR topPositionOffset = capsuleData.topPositon;

			//DrawCapsule3D(position, VAdd(position, topPositionOffset), radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			//DrawCapsule3D(nextPosition, VAdd(nextPosition, topPositionOffset), radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
		else if (kind == ColliderData::Kind::CHARACTER_CAPSULE)
		{
			//CharacterColliderData& capsuleData = dynamic_cast<CharacterColliderData&>(*item);
			//float radius = capsuleData.radius;
			//VECTOR topPositionOffset = capsuleData.topPositon;

			//DrawCapsule3D(position, VAdd(position, topPositionOffset), radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			//DrawCapsule3D(nextPosition, VAdd(nextPosition, topPositionOffset), radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
#endif
		//予定ポジション設定
		item->SetNextPosition(nextPosition);
	}

	/*当たり判定チェック*/
	CheckColide();

	/*位置確定*/
	FixPosition();

}

/// <summary>
/// 当たり判定チェック
/// </summary>
void CollisionManager::CheckColide()
{
	bool doCheck	= true;	//当たったか
	int  checkCount = 0;	//チェック回数

	/*当たっていたら*/
	while (doCheck)
	{
		//フラグを下す
		doCheck = false;
		//チェック回数を増やす
		checkCount++;
		//２重ループで全オブジェクト当たり判定
		//(重いのでオブジェクト同士のみ当たり判定するなど工夫がいる)
		for (auto& objectA : this->collidables)
		{
			for (auto& objectB : this->collidables)
			{
				//AとBが異なっていたら
				if (objectA != objectB)
				{
					//ぶつかっていれば
					if (IsCollide(*objectA, *objectB))
					{
						auto priorityA = objectA->GetPriority();
						auto priorityB = objectB->GetPriority();

						ColliderData* primary = objectA;
						ColliderData* secondary = objectB;
						//プライオリティの低いほうを移動
						if (priorityA < priorityB)
						{
							primary = objectB;
							secondary = objectA;
						}
						//次の座標を出す
						FixNextPosition(*primary, *secondary);

						doCheck = true;
						break;
					}
				}
			}
			if (doCheck)
			{
				break;
			}
		}
		//無限ループ避け
		if (checkCount > this->MAX_CHECK_COUNT && doCheck)
		{
#if _DEBUG
			printfDx("当たり判定の繰り返しチェックが規定数を超えた\n");
#endif
			break;
		}
	}
}

/// <summary>
/// 当たっているかどうかだけ判定
/// </summary>
bool CollisionManager::IsCollide(ColliderData& _objectA, ColliderData& _objectB)
{
	bool isHit = false;

	/*collidableの種類によって当たり判定を分ける*/
	auto aKind = _objectA.GetKind();
	auto bKind = _objectB.GetKind();

	/*球と球*/
	if (aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::SPHERE)
	{
		//auto aTob = VSub(_objectB.GetNextPosition(), _objectA.GetNextPosition());
		//auto aTobLength = VSize(aTob);

		///*互いの距離が、それぞれの半径の合計よりも小さければ当たる*/
		//auto objectAColliderData = dynamic_cast<ColliderDataSphere*>(_objectA);
		//auto objectBColliderData = dynamic_cast<ColliderDataSphere*>(_objectB);
		//isHit = (aTobLength < objectAColliderData->radius + objectBColliderData->radius);
	}

	/*カプセルとカプセル*/
	else if (aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto aTob = VSub(_objectB.GetNextPosition(), _objectA.GetNextPosition());
		auto aTobLength = VSize(aTob);

		/*互いの距離が、それぞれの半径の合計よりも小さければ当たる*/
		auto& objectAColliderData = dynamic_cast<CharacterColliderData&>(_objectA);
		auto& objectBColliderData = dynamic_cast<CharacterColliderData&>(_objectB);
		isHit = (aTobLength < objectAColliderData.radius + objectBColliderData.radius);
	}

	/*球とカプセル*/
	else if ((aKind == ColliderData::Kind::ATTACK_SPHERE && bKind == ColliderData::Kind::CHARACTER_CAPSULE) ||
		(aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::ATTACK_SPHERE))
	{
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			ColliderData* attackDataBase = &_objectA;
			VECTOR attackSphereCenter = _objectA.GetNextPosition();
			ColliderData* characterDataBase = &_objectB;
			VECTOR characterCapsuleUnder = _objectB.GetNextPosition();
			if (bKind == ColliderData::Kind::ATTACK_SPHERE)
			{
				attackDataBase = &_objectB;
				attackSphereCenter = _objectB.GetNextPosition();
				characterDataBase = &_objectA;
				characterCapsuleUnder = _objectA.GetNextPosition();
			}
			auto& sphereColliderData = dynamic_cast<AttackSphereColliderData&>(*attackDataBase);
			auto& capsuleColliderData = dynamic_cast<CharacterColliderData&>(*characterDataBase);
			if (sphereColliderData.data->isDoHitCheck)
			{
				VECTOR characterCapsuleTop = VAdd(characterCapsuleUnder,capsuleColliderData.topPositon);
				if (HitCheck_Sphere_Capsule(attackSphereCenter, sphereColliderData.radius, characterCapsuleUnder, characterCapsuleTop, capsuleColliderData.radius))
				{
					sphereColliderData.OnHit(*capsuleColliderData.data);
					capsuleColliderData.OnHit(*sphereColliderData.data, sphereColliderData.GetNextPosition());
					if (sphereColliderData.GetTag() == GameObjectTag::BOSS_ATTACK && capsuleColliderData.GetTag() == GameObjectTag::PLAYER)
					{
						auto& primaryAttackData = dynamic_cast<BossAttackData&> (*sphereColliderData.data);
						capsuleColliderData.SetPlayerReaction(primaryAttackData.playerReaction);
					}
				}
			}
		}
	}

	/*カプセルとカプセル*/
	else if ((aKind == ColliderData::Kind::ATTACK_CAPSULE && bKind == ColliderData::Kind::CHARACTER_CAPSULE) ||
		(aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::ATTACK_CAPSULE))
	{
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			ColliderData* attackDataBase = &_objectA;
			VECTOR attackCapsuleUnder = _objectA.GetNextPosition();
			ColliderData* characterDataBase = &_objectB;
			VECTOR characterCapsuleUnder = _objectB.GetNextPosition();
			if (bKind == ColliderData::Kind::ATTACK_CAPSULE)
			{
				attackDataBase = &_objectB;
				attackCapsuleUnder = _objectB.GetNextPosition();
				characterDataBase = &_objectA;
				characterCapsuleUnder = _objectA.GetNextPosition();
			}
			auto& attackColliderData = dynamic_cast<AttackCapsuleColliderData&>(*attackDataBase);
			auto& characterColliderData = dynamic_cast<CharacterColliderData&>(*characterDataBase);
			if (attackColliderData.data->isDoHitCheck)
			{
				VECTOR attackCapsuleTop = attackColliderData.topPositon;
				VECTOR characterCapsuleTop = VAdd(characterCapsuleUnder, characterColliderData.topPositon);
				if (HitCheck_Capsule_Capsule(attackCapsuleUnder, attackCapsuleTop, attackColliderData.radius, characterCapsuleUnder, characterCapsuleTop, characterColliderData.radius))
				{
					attackColliderData.OnHit(*characterColliderData.data);
					characterColliderData.OnHit(*attackColliderData.data, attackColliderData.GetNextPosition());
					if (attackColliderData.GetTag() == GameObjectTag::BOSS_ATTACK && characterColliderData.GetTag() == GameObjectTag::PLAYER)
					{
						auto& primaryAttackData = dynamic_cast<BossAttackData&> (*attackColliderData.data);
						characterColliderData.SetPlayerReaction(primaryAttackData.playerReaction);
					}
				}
			}
		}
	}

	/*カプセルと平面の当たり判定*/
	else if ((aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::PLANE) ||
		(aKind == ColliderData::Kind::PLANE && bKind == ColliderData::Kind::CHARACTER_CAPSULE))
	{
		ColliderData* planeDataBase = &_objectA;
		VECTOR planeCenter = _objectA.rigidbody.GetPosition();
		VECTOR capsuleUnder = _objectB.GetNextPosition();
		if (bKind == ColliderData::Kind::PLANE)
		{
			planeDataBase = &_objectB;
			planeCenter = _objectB.GetNextPosition();
			capsuleUnder = _objectA.GetNextPosition();
		}
		auto& planeColliderData = dynamic_cast<PlaneColliderData&>(*planeDataBase);

		if (bKind == ColliderData::Kind::CHARACTER_CAPSULE)
		{
			capsuleUnder = _objectB.GetNextPosition();
		}
		/*今は地面が円形の平面を持っているので、当たり判定はY座標(０以下かどうか)と平面の中心座標が平面の半径居ないかを判定する*/
		float distance = VSize(VSub(capsuleUnder, planeCenter));
		if ((capsuleUnder.y < 0.0f) || (distance > planeColliderData.radius))
		{
			isHit = true;
		}
	}
	/*球と平面の当たり判定*/
	else if ((aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::PLANE) ||
		(aKind == ColliderData::Kind::PLANE && bKind == ColliderData::Kind::SPHERE))
	{
		//ColliderData* planeDataBase = &_objectA;
		//VECTOR planeCenter = _objectA.GetNextPosition();
		//VECTOR sphereCenter = _objectB.GetNextPosition();
		//if (bKind == ColliderData::Kind::PLANE)
		//{
		//	planeDataBase = &_objectB;
		//	planeCenter = _objectB.GetNextPosition();
		//	sphereCenter = _objectA.GetNextPosition();
		//}
		//auto& planeColliderData = dynamic_cast<PlaneColliderData&>(*planeDataBase);

		//if (bKind == ColliderData::Kind::SPHERE)
		//{
		//	sphereCenter = _objectB.GetNextPosition();
		//}
		///*今は地面が円形の平面を持っているので、当たり判定はY座標(０以下かどうか)と平面の中心座標が平面の半径居ないかを判定する*/
		//float distance = VSize(VSub(sphereCenter, planeCenter));
		//if ((sphereCenter.y < 0.0f) || (distance > planeColliderData.radius))
		//{
		//	isHit = true;
		//}
	}

	return isHit;
}


void CollisionManager::FixNextPosition(ColliderData& _primary, ColliderData& _secondary)
{
	/*当たり判定の種別ごとに補正方法を変える*/
	auto primaryKind = _primary.GetKind();
	auto secondaryKind = _secondary.GetKind();
	//球同士の位置補正
	if (primaryKind == ColliderData::Kind::SPHERE && secondaryKind == ColliderData::Kind::SPHERE)
	{
		//VECTOR primaryToSecondary = VSub(secondary.GetNextPosition(), primary.GetNextPosition());
		//VECTOR primaryToSecondaryNorm = VNorm(primaryToSecondary);

		//auto primaryColliderData = dynamic_cast<ColliderDataSphere*> (primary);
		//auto secondaryColliderData = dynamic_cast<ColliderDataSphere*> (secondary);
		////そのままだとちょうど当たる位置になるので少し余分に離す
		//float awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;
		//VECTOR primaryToNewSecondaryPosition = VScale(primaryToSecondaryNorm, awayDist);
		//VECTOR fixedPosition = VAdd(primary.GetNextPosition(), primaryToNewSecondaryPosition);
		//secondary.GetNextPosition() = fixedPosition;
	}
	//カプセル同士の位置補正
	else if (primaryKind == ColliderData::Kind::CHARACTER_CAPSULE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		VECTOR secondaryToPrimary = VSub( _primary.GetNextPosition(), _secondary.GetNextPosition());
		float  secondaryToPrimarySize = VSize(secondaryToPrimary);
		VECTOR secondaryToPrimaryNorm = VNorm(secondaryToPrimary);

		auto& primaryColliderData = dynamic_cast<CharacterColliderData&> (_primary);
		auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);

		//そのままだとちょうど当たる位置になるので少し余分に離す
		float awayDist = primaryColliderData.radius + secondaryColliderData.radius - secondaryToPrimarySize + 0.05f;
		VECTOR fixVector = VScale(secondaryToPrimaryNorm, awayDist);
		VECTOR fixedPosition = VAdd(_primary.GetNextPosition(), fixVector);
		_primary.SetNextPosition(fixedPosition);
	}
	//平面とカプセル(平面はSTATICなので、必ずprimaryがPLANEになる)
	else if (primaryKind == ColliderData::Kind::PLANE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto& primaryColliderData = dynamic_cast<PlaneColliderData&> (_primary);

		VECTOR fixedPosition = _secondary.GetNextPosition();
		VECTOR secondaryToPrimary = VSub(_secondary.GetNextPosition(), _primary.rigidbody.GetPosition());
		float secondaryToPrimarySize = VSize(secondaryToPrimary);
		float fixValue = 0.0f;
		//中心点からキャラクターの座標までのベクトルのサイズが、平面の半径よりも大きければ
		if (secondaryToPrimarySize > primaryColliderData.radius)
		{
			fixValue = primaryColliderData.radius - secondaryToPrimarySize;
			fixedPosition = VAdd(fixedPosition, VScale(VNorm(secondaryToPrimary), fixValue));
		}
		fixedPosition.y = 0.0f;
		_secondary.SetNextPosition(fixedPosition);
	}
	//平面とスフィア(平面はSTATICなので、必ずprimaryがPLANEになる)
	else if (primaryKind == ColliderData::Kind::PLANE && secondaryKind == ColliderData::Kind::SPHERE)
	{
		//auto& primaryColliderData = dynamic_cast<PlaneColliderData&> (_primary);

		//VECTOR fixedPosition = _secondary.GetNextPosition();
		//if (fixedPosition.y < 0.0f)
		//{
		//	fixedPosition.y = 0.0f;
		//}
		//_secondary.SetNextPosition(fixedPosition);
	}
	//球とカプセル(球はSTATICなので、必ずprimaryがSPHEREになる)
	else if (primaryKind == ColliderData::Kind::ATTACK_SPHERE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		//
		//auto& primaryColliderData = dynamic_cast<AttackSphereColliderData&> (_primary);
		//auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);
		//if (primaryColliderData.data->isDoHitCheck)
		//{
		//	primaryColliderData.OnHit(*secondaryColliderData.data);
		//	secondaryColliderData.OnHit(*primaryColliderData.data, primaryColliderData.GetNextPosition());
		//	if (_primary.GetTag() == GameObjectTag::BOSS_ATTACK && _secondary.GetTag() == GameObjectTag::PLAYER)
		//	{
		//		auto& primaryAttackData = dynamic_cast<BossAttackData&> (*primaryColliderData.data);
		//		secondaryColliderData.SetPlayerReaction(primaryAttackData.playerReaction);
		//	}
		//}
	}

	//球とカプセル(球はSTATICなので、必ずprimaryがSPHEREになる)
	else if (primaryKind == ColliderData::Kind::ATTACK_CAPSULE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		//auto& primaryColliderData = dynamic_cast<AttackCapsuleColliderData&> (_primary);
		//auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);
		//if (primaryColliderData.data->isDoHitCheck)
		//{
		//}
	}
	//else
	//{
	//	assert(0 && "許可されていない当たり判定の位置補正です");
	//}
}

/// <summary>
/// 位置確定
/// </summary>
void CollisionManager::FixPosition()
{
	for (auto& item : collidables)
	{
		//positionを更新するので、velocityもそこに移動するvelocityに修正
		VECTOR toFixedPosition = VSub(item->GetNextPosition(), item->rigidbody.GetPosition());
		item->rigidbody.SetVelocity(toFixedPosition);

		//位置確定
		item->rigidbody.SetPosition(item->GetNextPosition());
	}
}

