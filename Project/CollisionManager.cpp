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
/// �Փ˕��̓o�^
/// </summary>
void CollisionManager::Entry(ColliderData& _data)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(this->collidables.begin(), this->collidables.end(), &_data) != this->collidables.end());

	/*�o�^����Ă��Ȃ�������*/
	if (!found)
	{
		this->collidables.emplace_back(&_data);
	}

	/*�o�^����Ă�����G���[��f��*/
	else
	{
		//assert(0 && "�w���collidable�͓o�^�ς݂ł�");
	}
}

/// <summary>
/// �Փ˕��̓o�^����
/// </summary>
void CollisionManager::Exit(ColliderData& _data)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(this->collidables.begin(), this->collidables.end(), &_data) != this->collidables.end());

	/*�o�^����Ă�����*/
	if (!found)
	{
		this->collidables.remove(&_data);
	}

	/*�o�^����Ă��Ȃ�������G���[��f��*/
	else
	{
		//assert(0 && "�w���collidable�͓o�^����Ă��܂���");
	}

}

/// <summary>
/// �X�V(�o�^�I�u�W�F�N�g�̕����ړ��A�Փ˒ʒm)
/// </summary>
void CollisionManager::Update()
{
	/*�ړ�*/
	for (auto& item : this->collidables)
	{
		//�|�W�V�����Ɉړ��͂𑫂�
		VECTOR position = item->rigidbody.GetPosition();
		VECTOR velocity = item->rigidbody.GetVelocity();
		//�d�͂𗘗p����ݒ�Ȃ�A�d�͂�ǉ�����
		if (item->rigidbody.UseGravity())
		{
			velocity = VAdd(velocity, VGet(0.0f, this->GRAVITY, 0));
			//�ő�d�͉����x���傫��������N�����v
			if (velocity.y < this->MAX_GRAVITY_ACCEL)
			{
				velocity = VGet(velocity.x, this->MAX_GRAVITY_ACCEL, velocity.z);
			}
		}
		//���̍��W��Velocity���o��
		VECTOR nextPosition = VAdd(position, velocity);
		item->rigidbody.SetVelocity(velocity);
		//�\��|�W�V�����ݒ�
		item->SetNextPosition(nextPosition);
	}

	/*�����蔻��`�F�b�N*/
	CheckColide();

	/*�ʒu�m��*/
	FixPosition();
}

/// <summary>
/// �����蔻��`�F�b�N
/// </summary>
void CollisionManager::CheckColide()
{
	bool doCheck	= true;	//����������
	int  checkCount = 0;	//�`�F�b�N��

	/*�������Ă�����*/
	while (doCheck)
	{
		//�t���O������
		doCheck = false;
		//�`�F�b�N�񐔂𑝂₷
		checkCount++;
		//�Q�d���[�v�őS�I�u�W�F�N�g�����蔻��
		//(�d���̂ŃI�u�W�F�N�g���m�̂ݓ����蔻�肷��ȂǍH�v������)
		for (auto& objectA : this->collidables)
		{
			for (auto& objectB : this->collidables)
			{
				//A��B���قȂ��Ă�����
				if (objectA != objectB)
				{
					//�Ԃ����Ă����
					if (IsCollide(*objectA, *objectB))
					{
						auto priorityA = objectA->GetPriority();
						auto priorityB = objectB->GetPriority();

						ColliderData* primary = objectA;
						ColliderData* secondary = objectB;
						//�v���C�I���e�B�̒Ⴂ�ق����ړ�
						if (priorityA < priorityB)
						{
							primary = objectB;
							secondary = objectA;
						}
						//���̍��W���o��
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

		//�������[�v����
		if (checkCount > this->MAX_CHECK_COUNT || !doCheck)
		{
#if _DEBUG
			//printfDx("�����蔻��̌J��Ԃ��`�F�b�N���K�萔�𒴂���\n");
#endif
			break;
		}
	}
}

/// <summary>
/// �������Ă��邩�ǂ�����������
/// </summary>
bool CollisionManager::IsCollide(ColliderData& _objectA, ColliderData& _objectB)
{
	bool isHit = false;

	/*collidable�̎�ނɂ���ē����蔻��𕪂���*/
	auto aKind = _objectA.GetKind();
	auto bKind = _objectB.GetKind();

	/*���Ƌ�*/
	if (aKind == ColliderData::Kind::SPHERE && bKind == ColliderData::Kind::SPHERE)
	{
		//auto aTob = VSub(_objectB.GetNextPosition(), _objectA.GetNextPosition());
		//auto aTobLength = VSize(aTob);

		///*�݂��̋������A���ꂼ��̔��a�̍��v������������Γ�����*/
		//auto objectAColliderData = dynamic_cast<ColliderDataSphere*>(_objectA);
		//auto objectBColliderData = dynamic_cast<ColliderDataSphere*>(_objectB);
		//isHit = (aTobLength < objectAColliderData->radius + objectBColliderData->radius);
	}

	/*�J�v�Z���ƃJ�v�Z��*/
	else if (aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if (aTag != bTag || (aTag != GameObjectTag::BOSS || bTag != GameObjectTag::BOSS))
		{
			/*�݂��̋������A���ꂼ��̔��a�̍��v������������Γ�����*/
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

	/*���ƃJ�v�Z��*/
	else if ((aKind == ColliderData::Kind::ATTACK_SPHERE && bKind == ColliderData::Kind::CHARACTER_CAPSULE) ||
		(aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::ATTACK_SPHERE))
	{
		//�U���̎�ƍU�����󂯂鑤���قȂ��Ă�����
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			//�U�����Ǝ󂯂鑤�𔻕ʂ���
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
			//�R���C�_�[���L���X�g
			auto& attackColliderData = dynamic_cast<AttackSphereColliderData&>(*attackDataBase);
			auto& characterColliderData = dynamic_cast<CharacterColliderData&>(*characterDataBase);
			//�L�����N�^�[�Ƃ̓����蔻����s���Ȃ�
			if (characterColliderData.isUseCollWithChara)
			{
				//�����蔻�������󋵂Ȃ�
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

	/*�J�v�Z���ƃJ�v�Z��*/
	else if ((aKind == ColliderData::Kind::ATTACK_CAPSULE && bKind == ColliderData::Kind::CHARACTER_CAPSULE) ||
		(aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::ATTACK_CAPSULE))
	{
		//�U���̎�ƍU�����󂯂鑤���قȂ��Ă�����
		auto aTag = _objectA.GetTag();
		auto bTag = _objectB.GetTag();
		if ((aTag == GameObjectTag::BOSS && bTag == GameObjectTag::PLAYER_ATTACK) ||
			(aTag == GameObjectTag::PLAYER && bTag == GameObjectTag::BOSS_ATTACK) ||
			(aTag == GameObjectTag::BOSS_ATTACK && bTag == GameObjectTag::PLAYER) ||
			(aTag == GameObjectTag::PLAYER_ATTACK && bTag == GameObjectTag::BOSS))
		{
			//�U�����Ǝ󂯂鑤�𔻕ʂ���
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

			//�R���C�_�[���L���X�g
			auto& attackColliderData = dynamic_cast<AttackCapsuleColliderData&>(*attackDataBase);
			auto& characterColliderData = dynamic_cast<CharacterColliderData&>(*characterDataBase);
			//�L�����N�^�[�Ƃ̓����蔻���p�ӂ��Ă����
			if (characterColliderData.isUseCollWithChara)
			{
				//�����蔻�������󋵂Ȃ�
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

	/*�J�v�Z���ƕ��ʂ̓����蔻��*/
	else if ((aKind == ColliderData::Kind::CHARACTER_CAPSULE && bKind == ColliderData::Kind::PLANE) ||
		(aKind == ColliderData::Kind::PLANE && bKind == ColliderData::Kind::CHARACTER_CAPSULE))
	{
		//�n�ʂƃL�����N�^�[�𔻕ʂ���
		ColliderData* planeDataBase = &_objectA;
		VECTOR planeCenter = _objectA.rigidbody.GetPosition();
		VECTOR capsuleUnder = _objectB.GetNextPosition();
		if (bKind == ColliderData::Kind::PLANE)
		{
			planeDataBase = &_objectB;
			planeCenter = _objectB.GetNextPosition();
			capsuleUnder = _objectA.GetNextPosition();
		}
		//�R���C�_�[���L���X�g
		auto& planeColliderData = dynamic_cast<PlaneColliderData&>(*planeDataBase);
		if (bKind == ColliderData::Kind::CHARACTER_CAPSULE)
		{
			capsuleUnder = _objectB.GetNextPosition();
		}
		/*���͒n�ʂ��~�`�̕��ʂ������Ă���̂ŁA�����蔻���Y���W(�O�ȉ����ǂ���)�ƕ��ʂ̒��S���W�����ʂ̔��a���Ȃ����𔻒肷��*/
		float distance = VSize(VSub(capsuleUnder, planeCenter));
		if ((capsuleUnder.y < 0.0f) || (distance > planeColliderData.radius))
		{
			isHit = true;
		}
	}
	/*���ƕ��ʂ̓����蔻��*/
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
		///*���͒n�ʂ��~�`�̕��ʂ������Ă���̂ŁA�����蔻���Y���W(�O�ȉ����ǂ���)�ƕ��ʂ̒��S���W�����ʂ̔��a���Ȃ����𔻒肷��*/
		//float distance = VSize(VSub(sphereCenter, planeCenter));
		//if ((sphereCenter.y < 0.0f) || (distance > planeColliderData.radius))
		//{
		//	isHit = true;
		//}
	}
	else if (aKind == ColliderData::Kind::MODEL && bKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		//�t���O�������Ă�����
		auto& charaCollision = dynamic_cast<CharacterColliderData&>(_objectB);
		auto& modelCollision = dynamic_cast<ModelColliderData&>(_objectA);
		if (charaCollision.isUseCollWithGround && modelCollision.isDoHitCheck)
		{
			auto& model_coll = dynamic_cast<ModelColliderData&>(_objectA);
			//�\�z�����R���W�������ƃJ�v�Z���Ƃ̓����蔻������A�\���̂Ɋi�[����
			VECTOR moveVector = charaCollision.rigidbody.GetVelocity();
			MV1_COLL_RESULT_POLY_DIM hitPolyDim = MV1CollCheck_Sphere(model_coll.modelHandle, model_coll.frameIndex, charaCollision.GetNextPosition(), charaCollision.radius + VSize(moveVector));
			//�������Ă�����ȉ��̏������s��
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
	/*�����蔻��̎�ʂ��Ƃɕ␳���@��ς���*/
	auto primaryKind = _primary.GetKind();
	auto secondaryKind = _secondary.GetKind();
	//�����m�̈ʒu�␳
	if (primaryKind == ColliderData::Kind::SPHERE && secondaryKind == ColliderData::Kind::SPHERE)
	{
		//VECTOR primaryToSecondary = VSub(secondary.GetNextPosition(), primary.GetNextPosition());
		//VECTOR primaryToSecondaryNorm = VNorm(primaryToSecondary);

		//auto primaryColliderData = dynamic_cast<ColliderDataSphere*> (primary);
		//auto secondaryColliderData = dynamic_cast<ColliderDataSphere*> (secondary);
		////���̂܂܂��Ƃ��傤�Ǔ�����ʒu�ɂȂ�̂ŏ����]���ɗ���
		//float awayDist = primaryColliderData->radius + secondaryColliderData->radius + 0.0001f;
		//VECTOR primaryToNewSecondaryPosition = VScale(primaryToSecondaryNorm, awayDist);
		//VECTOR fixedPosition = VAdd(primary.GetNextPosition(), primaryToNewSecondaryPosition);
		//secondary.GetNextPosition() = fixedPosition;
	}
	//�J�v�Z�����m�̈ʒu�␳
	else if (primaryKind == ColliderData::Kind::CHARACTER_CAPSULE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		//VECTOR secondaryToPrimary = VSub( _primary.GetNextPosition(), _secondary.GetNextPosition());
		//float  secondaryToPrimarySize = VSize(secondaryToPrimary);
		//VECTOR secondaryToPrimaryNorm = VNorm(secondaryToPrimary);
		auto& primaryColliderData = dynamic_cast<CharacterColliderData&> (_primary);
		auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);
		if (!primaryColliderData.isUseCollWithChara || !secondaryColliderData.isUseCollWithChara)return;

		/*�v���C�}���[�̈ړ��x�N�g���̃T�C�Y���o��*/
		VECTOR primaryVelocity = _primary.rigidbody.GetVelocity();
		primaryVelocity.y = 0.0f;
		float primaryMoveVectorSize = VSize(primaryVelocity);

		/*�Q�̃J�v�Z���̒��_���擾*/
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
		//�J�v�Z�����m�̍ŋߐڋ��������
		SEGMENT_SEGMENT_RESULT result;
		Segment_Segment_Analyse(&primaryUnderPosition, &primaryTopPosition, &secondaryUnderPosition, &secondaryTopPosition, &result);
		//���a�̍��v����ŋߐڋ��������������߂��A���̂܂܂��Ƃ��傤�Ǔ�����ʒu�ɂȂ�̂ŏ����]���ɗ���
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
	//���ʂƃJ�v�Z��(���ʂ�STATIC�Ȃ̂ŁA�K��primary��PLANE�ɂȂ�)
	else if (primaryKind == ColliderData::Kind::PLANE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto& primaryColliderData = dynamic_cast<PlaneColliderData&> (_primary);

		VECTOR fixedPosition = _secondary.GetNextPosition();
		VECTOR secondaryToPrimary = VSub(_secondary.GetNextPosition(), _primary.rigidbody.GetPosition());
		float secondaryToPrimarySize = VSize(secondaryToPrimary);
		float fixValue = 0.0f;
		//���S�_����L�����N�^�[�̍��W�܂ł̃x�N�g���̃T�C�Y���A���ʂ̔��a�����傫�����
		if (secondaryToPrimarySize > primaryColliderData.radius)
		{
			fixValue = primaryColliderData.radius - secondaryToPrimarySize;
			fixedPosition = VAdd(fixedPosition, VScale(VNorm(secondaryToPrimary), fixValue));
		}
		fixedPosition.y = 0.0f;
		_secondary.SetNextPosition(fixedPosition);
	}
	//���ʂƃX�t�B�A(���ʂ�STATIC�Ȃ̂ŁA�K��primary��PLANE�ɂȂ�)
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
	//���ƃJ�v�Z��(����STATIC�Ȃ̂ŁA�K��primary��SPHERE�ɂȂ�)
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

	//���ƃJ�v�Z��(����STATIC�Ȃ̂ŁA�K��primary��SPHERE�ɂȂ�)
	else if (primaryKind == ColliderData::Kind::ATTACK_CAPSULE && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		//auto& primaryColliderData = dynamic_cast<AttackCapsuleColliderData&> (_primary);
		//auto& secondaryColliderData = dynamic_cast<CharacterColliderData&> (_secondary);
		//if (primaryColliderData.data->isDoHitCheck)
		//{
		//}
	}
	/*���f���ƃL�����N�^�[�J�v�Z���i���f����STATIC�Ȃ̂ŁA�K��primary��CHARACTER_CAPSULE�ɂȂ�j*/
	else if (primaryKind == ColliderData::Kind::MODEL && secondaryKind == ColliderData::Kind::CHARACTER_CAPSULE)
	{
		auto& ModelCollision = dynamic_cast<ModelColliderData&>(_primary);
		auto& charaCollision = dynamic_cast<CharacterColliderData&>(_secondary);
		//�t���O�������Ă�����
		if (!charaCollision.isUseCollWithGround)return;
		//�\�z�����R���W�������ƃJ�v�Z���Ƃ̓����蔻������A�\���̂Ɋi�[����
		MV1_COLL_RESULT_POLY_DIM hitPolyDim = MV1CollCheck_Sphere(ModelCollision.modelHandle, ModelCollision.frameIndex, charaCollision.GetNextPosition(), charaCollision.radius);
		//�������Ă�����ȉ��̏������s��
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

			//�������Ă��鐔�������[�v����
			JudgeNorm(hitPolyDim, floor, xpWall, xmWall, zpWall, zmWall, charaCollision.rigidbody.GetPosition());

			//�ő�␳���W
			VECTOR maxfix = VGet(0.0f, 0.0f, 0.0f);
			//����������
			bool isHit = false;
			//�������Ă���|���S���̒��ŁA���Ɣ��肳�ꂽ�|���S���̏���
			for (int i = 0; i < floor.size(); i++)
			{
				//�O�p�`�Ɛ����̓����蔻����s��
				HITRESULT_LINE hitResult = HitCheck_Line_Triangle(charaCollision.GetNextPosition(), VAdd(charaCollision.GetNextPosition(), VGet(0.0f, hit_height, 0.0f)), floor[i].Position[0], floor[i].Position[1], floor[i].Position[2]);
				//�������Ă�����
				if (hitResult.HitFlag)
				{
					//�q�b�g�t���O�𗧂Ă�
					isHit = true;
					float fixY = hitResult.Position.y - charaCollision.GetNextPosition().y;
					//�������ۑ�����Ă���␳�ʂ����傫��������
					if (fixY > maxfix.y)
					{
						//�␳�ʂ�������
						maxfix.y = fixY;
					}
				}
			}
			//���ɓ������Ă�����A�����Ԃ�
			if (isHit)
			{
				newNextPosition.y = nowNextPosition.y + maxfix.y + 0.001f;
			}
			//======================================================================
			// ��������ǂ̏����ɓ���B
			// �ǂ̕ǂƓ��������̂��A���Ǝ�ނ𒲂ׂ�
			// �v���C���[�̈ړ��x�N�g���̂����AZ,X�̂ǂꂪ�傫�����𒲂�
			// ���Ƀ}�C�i�X���v���X�����ׂ�
			//======================================================================
			VECTOR	moveVector = charaCollision.rigidbody.GetDirection();
			int		hitWallNum = 0;
			bool	isHitXWall = false;
			bool	isHitZWall = false;
			//�ǂ̕ǂƓ������Ă���̂��A���Ǝ�ނ𒲂ׂ�
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
			//3��
			printfDx("���������ǂ̐�%d\n",hitWallNum);
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
				//�����ł�X�̓����蔻������
				if (isHitXWall)
				{
					//�ړ��x�N�g����X���v���X�̎��ɁAX�}�C�i�X�̕ǂƓ���������
					CheckHitXWall(xpWall, xmWall, isHit, maxfix, hit_height, radius, nowNextPosition, newNextPosition, moveVector);
				}
				//Z�̓����蔻������
				if (isHitZWall)
				{
					//�ړ��x�N�g����Z���v���X�̎��ɁAZ�}�C�i�X�̕ǂƓ���������
					CheckHitZWall(zpWall, zmWall, isHit, maxfix, hit_height, radius, nowNextPosition, newNextPosition, moveVector);
				}
			}
			//�����ł�X�̓����蔻������
			else if (isHitXWall)
			{
				//�ړ��x�N�g����X���v���X�̎��ɁAX�}�C�i�X�̕ǂƓ���������
				CheckHitXWall(xpWall, xmWall, isHit, maxfix, hit_height, radius, nowNextPosition, newNextPosition, moveVector);
			}
			//Z�̓����蔻������
			else if(isHitZWall)
			{
				//�ړ��x�N�g����Z���v���X�̎��ɁAZ�}�C�i�X�̕ǂƓ���������
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

			/*�i�[�����\���̂�j������*/
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
	//	assert(0 && "������Ă��Ȃ������蔻��̈ʒu�␳�ł�");
	//}
}

/// <summary>
/// �ʒu�m��
/// </summary>
void CollisionManager::FixPosition()
{
	for (auto& item : collidables)
	{
		//position���X�V����̂ŁAvelocity�������Ɉړ�����velocity�ɏC��
		VECTOR toFixedPosition = VSub(item->GetNextPosition(), item->rigidbody.GetPosition());
		item->rigidbody.SetVelocity(toFixedPosition);

		//�ʒu�m��
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
/// ���������|���S���̖@�����A�����ǂ��𒲂ׂ�
/// </summary>
void CollisionManager::JudgeNorm(const MV1_COLL_RESULT_POLY_DIM _hitPolyDim, vector<MV1_COLL_RESULT_POLY>& _floor, vector<MV1_COLL_RESULT_POLY>& _xpWall, vector<MV1_COLL_RESULT_POLY>& _xmWall, vector<MV1_COLL_RESULT_POLY>& _zpWall, vector<MV1_COLL_RESULT_POLY>& _zmWall, const VECTOR _position)
{
	/*�������Ă���|���S���̖@���𒲂ׂ�*/
	for (int i = 0; i < _hitPolyDim.HitNum; i++)
	{
		VECTOR norm = VNorm(_hitPolyDim.Dim[i].Normal);
		//������
		if (JudgeDegree(norm, FLOOR_NORM))
		{
			_floor.push_back(_hitPolyDim.Dim[i]);
		}
		//�ǔ���
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