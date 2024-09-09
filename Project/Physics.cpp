#include <cassert>
#include <DxLib.h>
#include "GoriLib.h"
#include "UseMath.h"

using namespace GoriLib;

/// <summary>
/// 衝突物の登録
/// </summary>
void Physics::Entry(Collidable* _collidable)
{
	/*登録されているか調べる*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*登録されていなかったら*/
	if (!found)
	{
		collidables.emplace_back(_collidable);
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
void Physics::Exit(Collidable* _collidable)
{
	/*登録されているか調べる*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*登録されていたら*/
	if (!found)
	{
		collidables.remove(_collidable);
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
void Physics::Update()
{
	//移動
	for (auto& item : collidables)
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
				velocity = VGet(velocity.x, this->MAX_GRAVITY_ACCEL, velocity.y);
			}
		}

		auto nextPosition = VAdd(position, velocity);
		item->rigidbody.SetVelocity(velocity);

		//もともとの情報、予定情報をデバッグ表示
#if _DEBUG
		auto kind = item->colliderData->GetKind();
		if (kind == ColliderData::Kind::SPHERE)
		{
			ColliderDataSphere* sphereData;
			sphereData = dynamic_cast<ColliderDataSphere*>(item->colliderData);
			float radius = sphereData->radius;
			DrawSphere3D(position, radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			DrawSphere3D(nextPosition, radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
		else if (kind == ColliderData::Kind::CAPSULE)
		{
			ColliderDataCapsule* capsuleData;
			capsuleData = dynamic_cast<ColliderDataCapsule*>(item->colliderData);
			float radius = capsuleData->radius;
			VECTOR topPositionOffset = VGet(0.0f, capsuleData->height, 0.0f);

			DrawCapsule3D(position, VAdd(position, topPositionOffset), radius, this->DIV_NUM, this->BEFORE_FIX_INFO_COLOR, this->BEFORE_FIX_INFO_COLOR, FALSE);
			DrawCapsule3D(nextPosition, VAdd(nextPosition, topPositionOffset), radius, this->DIV_NUM, this->AIM_INFO_COLOR, this->AIM_INFO_COLOR, FALSE);
		}
#endif
		//予定ポジション設定
		item->nextPosition = nextPosition;
	}
	//当たり判定チェック
	std::vector<OnCollideInfo> onCollideInfo = CheckColide();

	//位置確定
	FixPosition();

	//当たり通知
	for (auto& item : onCollideInfo)
	{
		item.owner->OnCollide(*item.colider);
	}
}

/// <summary>
/// 当たり判定チェック
/// </summary>
std::vector<Physics::OnCollideInfo> Physics::CheckColide()const
{
	std::vector<OnCollideInfo> onCollideInfo;
	//衝突通知、ポジション補正
	bool doCheck = true;
	int checkCount = 0;//チェック回数

	while (doCheck)
	{
		doCheck = false;
		checkCount++;

		//２重ループで全オブジェクト当たり判定
		//(重いのでオブジェクト同士のみ当たり判定するなど工夫がいる)
		for (auto& objectA : this->collidables)
		{
			for (auto& objectB : this->collidables)
			{
				if (objectA != objectB)
				{
					//ぶつかっていれば
					if (IsCollide(objectA, objectB))
					{
						auto priorityA = objectA->GetPriority();
						auto priorityB = objectB->GetPriority();

						Collidable* primary = objectA;
						Collidable* secondary = objectB;

						//どちらもトリガー出なければ次目標位置修正
						bool isTriggerAorB = objectA->colliderData->IsTrigger() || objectB->colliderData->IsTrigger();
						if (!isTriggerAorB)
						{
							//プライオリティの低いほうを移動
							if (priorityA < priorityB)
							{
								primary = objectB;
								secondary = objectA;
							}
							FixNextPosition(primary, secondary);
						}

						//衝突情報の更新
						//（primaryもsecondary藻何回も呼ばれる可能性はあるので、排他遅延処理）
						bool hasPrimaryInfo = false;
						bool hasSecondaryInfo = false;
						for (const auto& item : onCollideInfo)
						{
							//すでに通知リストに含まれていたら呼ばない
							if (item.owner == primary)
							{
								hasPrimaryInfo = true;
							}
							if (item.owner == secondary)
							{
								hasPrimaryInfo = true;
							}
						}
						if (!hasPrimaryInfo)
						{
							onCollideInfo.push_back({ primary,secondary });
						}
						if (!hasSecondaryInfo)
						{
							onCollideInfo.push_back({ secondary,primary });
						}

						//一度でもヒット＋補正したら衝突判定と補正やり直し
						//片方がトリガーならヒット取り直さない
						if (!isTriggerAorB)
						{
							doCheck = true;
						}
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
		if (checkCount > MAX_CHECK_COUNT && doCheck)
		{
#if _DEBUG
			printfDx("当たり判定の繰り返しチェックが規定数を超えた\n");
#endif
			break;
		}
	}
	return onCollideInfo;
}

/// <summary>
/// 指定ラインがオブジェクトとぶつかっているかどうか判定し、ぶつかっているオブジェクトを返す
/// </summary>
std::list<Collidable*> Physics::IsCollideLine(const VECTOR& _start, const VECTOR& _end)const
{
	std::list<Collidable*> ret;

	for (auto& object : collidables)
	{
		//collidableの種類によって、当たり判定を分ける
		auto kind = object->colliderData->GetKind();
		//ラインと平面
		if (kind == ColliderData::Kind::PLANE)
		{
			auto planeColliderData = dynamic_cast<ColliderDataPlane*>(object->colliderData);
			//ラインと点の最短距離を求める
			float minLength = Segment_Point_MinLength(_start, _end, object->rigidbody.GetPosition());
			//floatの誤差を考慮して判定
			if (minLength < 0.001f)
			{
				ret.emplace_back(object);
			}
		}
	}
	return ret;
}

/// <summary>
/// 当たっているかどうかだけ判定
/// </summary>
bool Physics::IsCollide(const Collidable* _objectA, const Collidable* _objectB)const
{
	bool isHit = false;

	/*どちらかのスルー対象としてタグが入っていたら無視*/
	if (_objectA->IsThroughTarget(_objectB) || _objectB->IsThroughTarget(_objectA))
	{
		return false;
	}

	/*collidableの種類によって当たり判定を分ける*/
	auto aKind = _objectA->colliderData->GetKind();
	auto bKind = _objectB->colliderData->GetKind();

	/*球と球*/
	if (aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::SPHERE)
	{
		//auto aTob = VSub(_objectB->nextPosition, _objectA->nextPosition);
		//auto aTobLength = VSize(aTob);

		///*互いの距離が、それぞれの半径の合計よりも小さければ当たる*/
		//auto objectAColliderData = dynamic_cast<ColliderDataSphere*>(_objectA->colliderData);
		//auto objectBColliderData = dynamic_cast<ColliderDataSphere*>(_objectB->colliderData);
		//isHit = (aTobLength < objectAColliderData->radius + objectBColliderData->radius);
	}
	/*カプセルとカプセル*/
	else if (aKind == ColliderData::Kind::CAPSULE && bKind == ColliderData::Kind::CAPSULE)
	{
		auto aTob = VSub(_objectB->nextPosition, _objectA->nextPosition);
		auto aTobLength = VSize(aTob);

		/*互いの距離が、それぞれの半径の合計よりも小さければ当たる*/
		auto objectAColliderData = dynamic_cast<ColliderDataCapsule*>(_objectA->colliderData);
		auto objectBColliderData = dynamic_cast<ColliderDataCapsule*>(_objectB->colliderData);
		isHit = (aTobLength < objectAColliderData->radius + objectBColliderData->radius);
	}

	/*球とカプセル*/
	else if ((aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::CAPSULE) ||
		(aKind == ColliderData::Kind::CAPSULE && bKind == ColliderData::Kind::SPHERE))
	{
		auto aTag = _objectA->GetTag();
		auto bTag = _objectB->GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			ColliderData* sphereDataBase = _objectA->colliderData;
			VECTOR sphereCenter = _objectA->nextPosition;
			ColliderData* capsuleDataBase = _objectB->colliderData;
			VECTOR capsuleUnder = _objectB->nextPosition;
			if (bKind == ColliderData::Kind::SPHERE)
			{
				sphereDataBase = _objectB->colliderData;
				sphereCenter = _objectB->nextPosition;
				capsuleDataBase = _objectA->colliderData;
				capsuleUnder = _objectA->nextPosition;
			}
			auto sphereColliderData = dynamic_cast<ColliderDataSphere*>(sphereDataBase);
			auto capsuleColliderData = dynamic_cast<ColliderDataCapsule*>(capsuleDataBase);
			if (sphereColliderData->GetHitNumber() != capsuleColliderData->GetHitNumber())
			{
				VECTOR capsuleTop = VGet(capsuleUnder.x, capsuleUnder.y + capsuleColliderData->height, capsuleUnder.z);
				isHit = HitCheck_Sphere_Capsule(sphereCenter, sphereColliderData->radius, capsuleUnder, capsuleTop, capsuleColliderData->radius);
			}
		}
	}
	/*カプセルと平面の当たり判定*/
	else if ((aKind == ColliderData::Kind::CAPSULE && bKind == ColliderData::Kind::PLANE) ||
		(aKind == ColliderData::Kind::PLANE && bKind == ColliderData::Kind::CAPSULE))
	{
		ColliderData* planeDataBase = _objectA->colliderData;
		VECTOR planeCenter = _objectA->rigidbody.GetPosition();
		VECTOR capsuleUnder = _objectB->nextPosition;
		if (bKind == ColliderData::Kind::PLANE)
		{
			planeDataBase = _objectB->colliderData;
			planeCenter = _objectB->nextPosition;
			capsuleUnder = _objectA->nextPosition;
		}
		auto planeColliderData = dynamic_cast<ColliderDataPlane*>(planeDataBase);

		if (bKind == ColliderData::Kind::CAPSULE)
		{
			capsuleUnder = _objectB->nextPosition;
		}
		/*今は地面が円形の平面を持っているので、当たり判定はY座標(０以下かどうか)と平面の中心座標が平面の半径居ないかを判定する*/
		float distance = VSize(VSub(capsuleUnder, planeCenter));
		if ((capsuleUnder.y < 0.0f) || (distance > planeColliderData->radius))
		{
			isHit = true;
		}
	}
	return isHit;
}


void Physics::FixNextPosition(Collidable* primary, Collidable* secondary)const
{
	/*当たり判定の種別ごとに補正方法を変える*/
	auto primaryKind = primary->colliderData->GetKind();
	auto secondaryKind = secondary->colliderData->GetKind();

	//球同士の位置補正
	if (primaryKind == ColliderData::Kind::SPHERE && secondaryKind == ColliderData::Kind::SPHERE)
	{
		//VECTOR primaryToSecondary = VSub(secondary->nextPosition, primary->nextPosition);
		//VECTOR primaryToSecondaryNorm = VNorm(primaryToSecondary);

		//auto primaryColliderData = dynamic_cast<ColliderDataSphere*> (primary->colliderData);
		//auto secondaryColliderData = dynamic_cast<ColliderDataSphere*> (secondary->colliderData);
		////そのままだとちょうど当たる位置になるので少し余分に離す
		//float awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;
		//VECTOR primaryToNewSecondaryPosition = VScale(primaryToSecondaryNorm, awayDist);
		//VECTOR fixedPosition = VAdd(primary->nextPosition, primaryToNewSecondaryPosition);
		//secondary->nextPosition = fixedPosition;
	}
	//カプセル同士の位置補正
	else if (primaryKind == ColliderData::Kind::CAPSULE && secondaryKind == ColliderData::Kind::CAPSULE)
	{
		VECTOR primaryToSecondary = VSub(secondary->nextPosition, primary->nextPosition);
		VECTOR primaryToSecondaryNorm = VNorm(primaryToSecondary);

		auto primaryColliderData = dynamic_cast<ColliderDataCapsule*> (primary->colliderData);
		auto secondaryColliderData = dynamic_cast<ColliderDataCapsule*> (secondary->colliderData);
		//そのままだとちょうど当たる位置になるので少し余分に離す
		float awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;
		VECTOR primaryToNewSecondaryPosition = VScale(primaryToSecondaryNorm, awayDist);
		VECTOR fixedPosition = VAdd(primary->nextPosition, primaryToNewSecondaryPosition);
		secondary->nextPosition = fixedPosition;
	}
	//平面とカプセル(平面はSTATICなので、必ずprimaryがPLANEになる)
	else if (primaryKind == ColliderData::Kind::PLANE && secondaryKind == ColliderData::Kind::CAPSULE)
	{
		auto primaryColliderData = dynamic_cast<ColliderDataPlane*> (primary->colliderData);

		VECTOR fixedPosition = secondary->nextPosition;
		VECTOR secondaryToPrimary = VSub(secondary->nextPosition, primary->rigidbody.GetPosition());
		float distance = VSize(secondaryToPrimary);
		float fixValue = 0.0f;

		if (distance > primaryColliderData->radius)
		{
			fixValue = primaryColliderData->radius - distance;
			fixedPosition = VAdd(fixedPosition, VScale(VNorm(secondaryToPrimary), fixValue));
		}
		fixedPosition.y = 0.0f;
		secondary->nextPosition = fixedPosition;
	}
	//球とカプセル(球はSTATICなので、必ずprimaryがSPHEREになる)
	else if (primaryKind == ColliderData::Kind::SPHERE && secondaryKind == ColliderData::Kind::CAPSULE)
	{
		auto primaryColliderData = dynamic_cast<ColliderDataSphere*> (primary->colliderData);
		auto secondaryColliderData = dynamic_cast<ColliderDataCapsule*> (secondary->colliderData);
		if (secondaryColliderData->hitNumber != primaryColliderData->hitNumber)
		{
			if (!secondaryColliderData->isCutDamage)
			{
				secondaryColliderData->hp -= primaryColliderData->damage;
			}
			secondaryColliderData->hitNumber = primaryColliderData->hitNumber;
		}
		
	}
	else
	{
		assert(0 && "許可されていない当たり判定の位置補正です");
	}
}

/// <summary>
/// 位置確定
/// </summary>
void Physics::FixPosition()
{
	for (auto& item : collidables)
	{
		//positionを更新するので、velocityもそこに移動するvelocityに修正
		VECTOR toFixedPosition = VSub(item->nextPosition, item->rigidbody.GetPosition());
		item->rigidbody.SetVelocity(toFixedPosition);

		//位置確定
		item->rigidbody.SetPosition(item->nextPosition);
	}
}

void Physics::DamageCalc(Collidable* _primary, Collidable* _secondary)const
{

}