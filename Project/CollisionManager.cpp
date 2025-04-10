#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "UseMath.h"
#include "GameObjectTag.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "ColliderData.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
#include "PlaneColliderData.h"
#include "AttackCapsuleColliderData.h"
#include "AttackSphereColliderData.h"
#include "CharacterColliderData.h"
#include "ModelColliderData.h"
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
		//assert(0 && "指定のcollidableは登録済みです");
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
		//assert(0 && "指定のcollidableは登録されていません");
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
		VECTOR position = item->rigidbody.GetPosition();
		VECTOR velocity = item->rigidbody.GetVelocity();
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
		VECTOR nextPosition = VAdd(position, velocity);
		item->rigidbody.SetVelocity(velocity);
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

						if (objectA->GetKind() != ColliderData::Kind::MODEL && objectB->GetKind() != ColliderData::Kind::MODEL)
						{
							doCheck = true;
							break;
						}
					}
				}
			}
			if (doCheck)
			{
				break;
			}
		}

		//無限ループ避け
		if (checkCount > this->MAX_CHECK_COUNT || !doCheck)
		{
#if _DEBUG
			//printfDx("当たり判定の繰り返しチェックが規定数を超えた\n");
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
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if (aTag != bTag || (aTag != GameObjectTag::BOSS || bTag != GameObjectTag::BOSS))
		{
			/*互いの距離が、それぞれの半径の合計よりも小さければ当たる*/
			auto& objectAColliderData = dynamic_cast<CharacterColliderData&>(_objectA);
			auto& objectBColliderData = dynamic_cast<CharacterColliderData&>(_objectB);
			if ((objectAColliderData.data->hp > 0 && objectBColliderData.data->hp > 0) && (objectAColliderData.isUseCollWithChara && objectBColliderData.isUseCollWithChara))
			{
				VECTOR aUnderPosition = objectAColliderData.GetNextPosition();
				VECTOR aTopPosition = objectAColliderData.topPositon;
				if (!objectAColliderData.isSetTopPosition)
				{
					aTopPosition = VAdd(aUnderPosition, objectAColliderData.topPositon);
				}
				VECTOR bUnderPosition = objectBColliderData.GetNextPosition();
				VECTOR bTopPosition = objectBColliderData.topPositon;
				if (!objectBColliderData.isSetTopPosition)
				{
					bTopPosition = VAdd(bUnderPosition, objectBColliderData.topPositon);
				}
				if (HitCheck_Capsule_Capsule(aUnderPosition, aTopPosition, objectAColliderData.radius, bUnderPosition, bTopPosition, objectBColliderData.radius))
				{
					isHit = true;
				}
			}
		}
	}

	/*球とカプセル*/
	else if ((aKind == ColliderData::Kind::ATTACK_SPHERE && bKind == ColliderData::Kind::CHARACTER_CAPSULE) ||
		(aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::ATTACK_SPHERE))
	{
		//攻撃の主と攻撃を受ける側が異なっていたら
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			//攻撃側と受ける側を判別する
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
			//コライダーをキャスト
			auto& attackColliderData = dynamic_cast<AttackSphereColliderData&>(*attackDataBase);
			auto& characterColliderData = dynamic_cast<CharacterColliderData&>(*characterDataBase);
			//キャラクターとの当たり判定を行うなら
			if (characterColliderData.isUseCollWithChara)
			{
				//当たり判定を取れる状況なら
				if (attackColliderData.data->isDoHitCheck && !characterColliderData.data->isInvinvible)
				{
					VECTOR characterCapsuleTop;
					if (characterColliderData.isSetTopPosition)
					{
						characterCapsuleTop = characterColliderData.topPositon;
					}
					else
					{
						characterCapsuleTop = VAdd(characterCapsuleUnder, characterColliderData.topPositon);
					}
					if (HitCheck_Sphere_Capsule(attackSphereCenter, attackColliderData.radius, characterCapsuleUnder, characterCapsuleTop, characterColliderData.radius))
					{
						attackColliderData.OnHit(*characterColliderData.data);
						characterColliderData.OnHit(*attackColliderData.data, characterColliderData.GetNextPosition());
					}
				}
			}
		}
	}

	/*カプセルとカプセル*/
	else if ((aKind == ColliderData::Kind::ATTACK_CAPSULE && bKind == ColliderData::Kind::CHARACTER_CAPSULE) ||
		(aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::ATTACK_CAPSULE))
	{
		//攻撃の主と攻撃を受ける側が異なっていたら
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			//攻撃側と受ける側を判別する
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

			//コライダーをキャスト
			auto& attackColliderData = dynamic_cast<AttackCapsuleColliderData&>(*attackDataBase);
			auto& characterColliderData = dynamic_cast<CharacterColliderData&>(*characterDataBase);
			//キャラクターとの当たり判定を用意していれば
			if (characterColliderData.isUseCollWithChara)
			{
				//当たり判定を取れる状況なら
				if (attackColliderData.data->isDoHitCheck && !characterColliderData.data->isInvinvible)
				{
					VECTOR attackCapsuleTop = attackColliderData.topPositon;
					VECTOR characterCapsuleTop;
					if (characterColliderData.isSetTopPosition)
					{
						characterCapsuleTop = characterColliderData.topPositon;
					}
					else
					{
						characterCapsuleTop = VAdd(characterCapsuleUnder, characterColliderData.topPositon);
					}
					if (HitCheck_Capsule_Capsule(attackCapsuleUnder, attackCapsuleTop, attackColliderData.radius, characterCapsuleUnder, characterCapsuleTop, characterColliderData.radius))
					{
						attackColliderData.OnHit(*characterColliderData.data);
						characterColliderData.OnHit(*attackColliderData.data, attackColliderData.GetNextPosition());
					}
				}
			}
		}
	}

	/*カプセルと平面の当たり判定*/
	else if ((aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::PLANE) ||
		(aKind == ColliderData::Kind::PLANE && bKind == ColliderData::Kind::CHARACTER_CAPSULE))
	{
		//地面とキャラクターを判別する
		ColliderData* planeDataBase = &_objectA;
		VECTOR planeCenter = _objectA.rigidbody.GetPosition();
		VECTOR capsuleUnder = _objectB.GetNextPosition();
		if (bKind == ColliderData::Kind::PLANE)
		{
			planeDataBase = &_objectB;
			planeCenter = _objectB.GetNextPosition();
			capsuleUnder = _objectA.GetNextPosition();
		}
		//コライダーをキャスト
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
	else if (aKind == ColliderData::Kind::MODEL && bKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		//フラグが立っていたら
		auto& charaCollision = dynamic_cast<CharacterColliderData&>(_objectB);
		auto& modelCollision = dynamic_cast<ModelColliderData&>(_objectA);
		if (charaCollision.isUseCollWithGround && modelCollision.isDoHitCheck)
		{
			auto& model_coll = dynamic_cast<ModelColliderData&>(_objectA);
			//構築したコリジョン情報とカプセルとの当たり判定を取り、構造体に格納する
			VECTOR moveVector = charaCollision.rigidbody.GetVelocity();
			MV1_COLL_RESULT_POLY_DIM hitPolyDim = MV1CollCheck_Sphere(model_coll.modelHandle, model_coll.frameIndex, charaCollision.GetNextPosition(), charaCollision.radius + VSize(moveVector));
			//当たっていたら以下の処理を行う
			if (hitPolyDim.HitNum > 0)
			{
				isHit = true;
			}
		}
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
		//VECTOR secondaryToPrimary = VSub( _primary.GetNextPosition(), _secondary.GetNextPosition());
		//float  secondaryToPrimarySize = VSize(secondaryToPrimary);
		//VECTOR secondaryToPrimaryNorm = VNorm(secondaryToPrimary);
		auto& primaryColliderData = dynamic_cast<CharacterColliderData&> (_primary);
		auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);
		if (!primaryColliderData.isUseCollWithChara || !secondaryColliderData.isUseCollWithChara)return;

		/*プライマリーの移動ベクトルのサイズを出す*/
		VECTOR primaryVelocity = _primary.rigidbody.GetVelocity();
		primaryVelocity.y = 0.0f;
		float primaryMoveVectorSize = VSize(primaryVelocity);

		/*２つのカプセルの頂点を取得*/
		VECTOR primaryUnderPosition = primaryColliderData.GetNextPosition();
		VECTOR primaryTopPosition = primaryColliderData.topPositon;
		if (!primaryColliderData.isSetTopPosition)
		{
			primaryUnderPosition.y = 0.0f;
			primaryTopPosition = VAdd(primaryUnderPosition, primaryColliderData.topPositon);
		}
		VECTOR secondaryUnderPosition = secondaryColliderData.GetNextPosition();
		VECTOR secondaryTopPosition = secondaryColliderData.topPositon;
		if (!secondaryColliderData.isSetTopPosition)
		{
			secondaryUnderPosition.y = 0.0f;
			secondaryTopPosition = VAdd(secondaryUnderPosition, secondaryColliderData.topPositon);
		}
		//カプセル同士の最近接距離を取る
		SEGMENT_SEGMENT_RESULT result;
		Segment_Segment_Analyse(&primaryUnderPosition, &primaryTopPosition, &secondaryUnderPosition, &secondaryTopPosition, &result);
		//半径の合計から最近接距離を引き押し戻す、そのままだとちょうど当たる位置になるので少し余分に離す
		float awayDist = primaryColliderData.radius + secondaryColliderData.radius - sqrt(result.SegA_SegB_MinDist_Square) + 0.000005f;
		
		if (awayDist > primaryMoveVectorSize)
		{
			awayDist = primaryMoveVectorSize;
		}

		VECTOR awayVector = VNorm(VSub(result.SegA_MinDist_Pos, result.SegB_MinDist_Pos));
		awayVector.y = 0.0f;
		VECTOR fixVector = VScale(awayVector, awayDist);
		VECTOR fixedPosition = VAdd(_primary.GetNextPosition(), fixVector);
		if (fixedPosition.y < -14.0f)
		{
			fixedPosition.y = -14.0f;
		}
		else if (fixedPosition.y > -13.8f)
		{
			fixedPosition.y = -13.8f;
		}
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
	/*モデルとキャラクターカプセル（モデルはSTATICなので、必ずprimaryがCHARACTER_CAPSULEになる）*/
	else if (primaryKind == ColliderData::Kind::MODEL && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto& ModelCollision = dynamic_cast<ModelColliderData&>(_primary);
		auto& charaCollision = dynamic_cast<CharacterColliderData&>(_secondary);
		//フラグが立っていたら
		if (!charaCollision.isUseCollWithGround)return;
		//構築したコリジョン情報とカプセルとの当たり判定を取り、構造体に格納する
		MV1_COLL_RESULT_POLY_DIM hitPolyDim = MV1CollCheck_Sphere(ModelCollision.modelHandle, ModelCollision.frameIndex, charaCollision.GetNextPosition(), charaCollision.radius);
		//当たっていたら以下の処理を行う
		if (hitPolyDim.HitNum > 0)
		{
			vector<MV1_COLL_RESULT_POLY> floor;
			vector<MV1_COLL_RESULT_POLY> xpWall;
			vector<MV1_COLL_RESULT_POLY> xmWall;
			vector<MV1_COLL_RESULT_POLY> zpWall;
			vector<MV1_COLL_RESULT_POLY> zmWall;
			VECTOR nowNextPosition = _secondary.GetNextPosition();
			VECTOR newNextPosition = _secondary.GetNextPosition();
			float  radius = charaCollision.radius;
			float  hit_height = charaCollision.topPositon.y;
			VECTOR velocity = _secondary.rigidbody.GetVelocity();

			//当たっている数だけループを回す
			JudgeNorm(hitPolyDim, floor, xpWall, xmWall, zpWall, zmWall, charaCollision.rigidbody.GetPosition());

			//最大補正座標
			VECTOR maxfix = VGet(0.0f, 0.0f, 0.0f);
			//当たったか
			bool isHit = false;
			//当たっているポリゴンの中で、床と判定されたポリゴンの処理
			for (int i = 0; i < floor.size(); i++)
			{
				//三角形と線分の当たり判定を行う
				HITRESULT_LINE hitResult = HitCheck_Line_Triangle(charaCollision.GetNextPosition(), VAdd(charaCollision.GetNextPosition(), VGet(0.0f, hit_height, 0.0f)), floor[i].Position[0], floor[i].Position[1], floor[i].Position[2]);
				//当たっていたら
				if (hitResult.HitFlag)
				{
					//ヒットフラグを立てる
					isHit = true;
					float fixY = hitResult.Position.y - charaCollision.GetNextPosition().y;
					//もし今保存されている補正量よりも大きかったら
					if (fixY > maxfix.y)
					{
						//補正量を代入する
						maxfix.y = fixY;
					}
				}
			}
			//床に当たっていたら、押し返す
			if (isHit)
			{
				newNextPosition.y = nowNextPosition.y + maxfix.y + 0.001f;
			}
			//======================================================================
			// ここから壁の処理に入る。
			// どの壁と当たったのか、数と種類を調べる
			// プレイヤーの移動ベクトルのうち、Z,Xのどれが大きいかを調べ
			// 次にマイナスかプラスか調べる
			//======================================================================
			VECTOR	moveVector = charaCollision.rigidbody.GetDirection();
			int		hitWallNum = 0;
			bool	isHitXWall = false;
			bool	isHitZWall = false;
			//どの壁と当たっているのか、数と種類を調べる
			if (xpWall.size() != 0)
			{
				++hitWallNum;
				isHitXWall = true;
			}
			if (xmWall.size() != 0)
			{
				++hitWallNum;
				isHitXWall = true;
			}
			if (zpWall.size() != 0)
			{
				++hitWallNum;
				isHitZWall = true;
			}
			if (zmWall.size() != 0)
			{
				++hitWallNum;
				isHitZWall = true;
			}
			//3つの
			printfDx("当たった壁の数%d\n",hitWallNum);
			if (hitWallNum >= 2)
			{
				if (hitWallNum == 3)
				{
					if (isHitXWall && xpWall.size() != 0 && xmWall.size())
					{
						isHitXWall = false;
					}
					if (isHitZWall && zpWall.size() != 0 && zmWall.size())
					{
						isHitZWall = false;
					}
				}
				//ここではXの当たり判定を取る
				if (isHitXWall)
				{
					//移動ベクトルのXがプラスの時に、Xマイナスの壁と当たったか
					CheckHitXWall(xpWall, xmWall, isHit, maxfix, hit_height, radius, nowNextPosition, newNextPosition, moveVector);
				}
				//Zの当たり判定を取る
				if (isHitZWall)
				{
					//移動ベクトルのZがプラスの時に、Zマイナスの壁と当たったか
					CheckHitZWall(zpWall, zmWall, isHit, maxfix, hit_height, radius, nowNextPosition, newNextPosition, moveVector);
				}
			}
			//ここではXの当たり判定を取る
			else if (isHitXWall)
			{
				//移動ベクトルのXがプラスの時に、Xマイナスの壁と当たったか
				CheckHitXWall(xpWall, xmWall, isHit, maxfix, hit_height, radius, nowNextPosition, newNextPosition, moveVector);
			}
			//Zの当たり判定を取る
			else if(isHitZWall)
			{
				//移動ベクトルのZがプラスの時に、Zマイナスの壁と当たったか
				CheckHitZWall(zpWall, zmWall, isHit, maxfix, hit_height, radius, nowNextPosition, newNextPosition, moveVector);
			}
			if (newNextPosition.y < -14.0f)
			{
				newNextPosition.y = newNextPosition.y;
			}
			else if (newNextPosition.y > -13.8f)
			{
				newNextPosition.y = -13.8f;
			}

			/*格納した構造体を破棄する*/
			MV1CollResultPolyDimTerminate(hitPolyDim);
			floor.clear();
			xpWall.clear();
			xmWall.clear();
			zpWall.clear();
			zmWall.clear();
			_secondary.SetNextPosition(newNextPosition);
		}
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

float CollisionManager::GetDegree(const VECTOR _norm1, const VECTOR _norm2)
{
	float dot = VDot(_norm1, _norm2);
	float deg = acosf(dot);
	return deg * 180.0f / DX_PI_F;
}

/// <summary>
/// 当たったポリゴンの法線が、床か壁かを調べる
/// </summary>
void CollisionManager::JudgeNorm(const MV1_COLL_RESULT_POLY_DIM _hitPolyDim, vector<MV1_COLL_RESULT_POLY>& _floor, vector<MV1_COLL_RESULT_POLY>& _xpWall, vector<MV1_COLL_RESULT_POLY>& _xmWall, vector<MV1_COLL_RESULT_POLY>& _zpWall, vector<MV1_COLL_RESULT_POLY>& _zmWall, const VECTOR _position)
{
	/*当たっているポリゴンの法線を調べる*/
	for (int i = 0; i < _hitPolyDim.HitNum; i++)
	{
		VECTOR norm = VNorm(_hitPolyDim.Dim[i].Normal);
		//床判定
		if (JudgeDegree(norm, FLOOR_NORM))
		{
			_floor.push_back(_hitPolyDim.Dim[i]);
		}
		//壁判定
		else if(_hitPolyDim.Dim[i].Position[0].y >= _position.y ||
				_hitPolyDim.Dim[i].Position[1].y >= _position.y ||
				_hitPolyDim.Dim[i].Position[2].y >= _position.y)
		{
			if (JudgeDegree(norm, X_PLUS_NORM))
			{
				_xpWall.push_back(_hitPolyDim.Dim[i]);
			}
			else if (JudgeDegree(norm, X_MINUS_NORM))
			{
				_xmWall.push_back(_hitPolyDim.Dim[i]);
			}
			else if (JudgeDegree(norm, Z_PLUS_NORM))
			{
				_zpWall.push_back(_hitPolyDim.Dim[i]);
			}
			else if (JudgeDegree(norm, Z_MINUS_NORM))
			{
				_zmWall.push_back(_hitPolyDim.Dim[i]);
			}
		}
	}
}
bool CollisionManager::JudgeDegree(const VECTOR _norm1, const VECTOR _norm2)
{
	float deg = GetDegree(_norm1, _norm2);
	if (deg < MAX_DEGREE)
	{
		return true;
	}
	return false;
}
float CollisionManager::JudgeMax(const float _nowMax, const float _judgeValue)
{
	if (_nowMax < _judgeValue)
	{
		return _judgeValue;
	}
	return _nowMax;
}
bool CollisionManager::HitCheckWall(float& _max, vector<MV1_COLL_RESULT_POLY> _wall, const VECTOR _pos, const float _height, const float _radius, const bool _sign, const bool isX, const bool isZ)
{
	bool isHit = false;
	for (int i = 0; i < _wall.size(); i++)
	{
		int hitResult = HitCheck_Capsule_Triangle(_pos, VAdd(_pos, VGet(0.0f, _height, 0.0f)), _radius, _wall[i].Position[0], _wall[i].Position[1], _wall[i].Position[2]);
		if (hitResult)
		{
			isHit = true;
			for (int j = 0; j < 3; j++)
			{
				if (_wall[i].Position[j].y > -0.000001f)
				{
					float fix = 0.0f;
					if (isX)
					{
						if (_wall[i].Position[j].x > _pos.x)
						{
							fix = _wall[i].Position[j].x - (_pos.x + _radius);
						}
						else
						{
							fix = _wall[i].Position[j].x - (_pos.x - _radius);
						}
					}
					else if (isZ)
					{
						if (_wall[i].Position[j].z > _pos.z)
						{
							fix = _wall[i].Position[j].z - (_pos.z + _radius);
						}
						else
						{
							fix = _wall[i].Position[j].z - (_pos.z - _radius);
						}
					}
					if (_sign)
					{
						if (fix < _max)
						{
							_max = fix;
						}
					}
					else
					{
						if (fix > _max)
						{
							_max = fix;
						}
					}
				}
			}
		}
	}

	return isHit;
}


void CollisionManager::CheckHitXWall(vector<MV1_COLL_RESULT_POLY> _xpWall, vector<MV1_COLL_RESULT_POLY> _xmWall,
	bool& _isHit, VECTOR& _maxFix, float _hitHeight, float _radius, VECTOR _nowNextPosition, VECTOR& _newNextPosition, VECTOR _moveVector)
{
	if (_moveVector.x < 0.0f && _xpWall.size() != 0)
	{
		_isHit = HitCheckWall(_maxFix.x, _xpWall, _nowNextPosition, _hitHeight, _radius, false, true, false);
		if (_isHit)
		{
			_newNextPosition.x = _nowNextPosition.x + _maxFix.x + 0.001f;
		}
	}
	else if (_moveVector.x > 0.0f && _xmWall.size() != 0)
	{
		_isHit = HitCheckWall(_maxFix.x, _xmWall, _nowNextPosition, _hitHeight, _radius, true, true, false);
		if (_isHit)
		{
			_newNextPosition.x = _nowNextPosition.x + _maxFix.x + 0.001f;
		}
	}
}
void CollisionManager::CheckHitZWall(vector<MV1_COLL_RESULT_POLY> _zpWall, vector<MV1_COLL_RESULT_POLY> _zmWall,
	bool& _isHit, VECTOR& _maxFix, float _hitHeight, float _radius, VECTOR _nowNextPosition, VECTOR& _newNextPosition, VECTOR _moveVector)
{
	if (_moveVector.z < 0.0f && _zpWall.size() != 0)
	{
		_isHit = HitCheckWall(_maxFix.z, _zpWall, _nowNextPosition, _hitHeight, _radius, false, false, true);
		if (_isHit)
		{
			_newNextPosition.z = _nowNextPosition.z + _maxFix.z + 0.001f;
		}
	}
	else if (_moveVector.z > 0.0f && _zmWall.size() != 0)
	{
		_isHit = HitCheckWall(_maxFix.z, _zmWall, _nowNextPosition, _hitHeight, _radius, true, false, true);
		if (_isHit)
		{
			_newNextPosition.z = _nowNextPosition.z + _maxFix.z + 0.001f;
		}
	}
}