#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "AttackSphereColliderData.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
AttackSphereColliderData::AttackSphereColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, AttackData* _data)
	: ColliderData	(ColliderData::Kind::ATTACK_SPHERE, _priority, _tag)
	, radius		(0.0f)
	, data			(_data)
{
	//�����Ȃ�
}

/// <summary>
/// �q�b�g����
/// </summary>
void AttackSphereColliderData::OnHit(const CharacterData& _data)
{
	//�U���q�b�g�`�F�b�N�t���O������
	this->data->isDoHitCheck = false;

	/*�J�[�h���������Ă��邩*/
	bool isGuard = false;
	if (_data.isGuard)
	{
		//�c��̃X�^�~�i������Ă�����K�[�h����
		if (_data.stamina >= this->data->blockStaminaConsumption)
		{
			isGuard = true;
		}
	}

	/*���G�t���O���K�[�h�����Ă��Ȃ�������*/
	if (!_data.isInvinvible && !isGuard)
	{
		//�U���ɓ��������t���O�𗧂Ă�
		this->data->isHitAttack = true;
	}
}