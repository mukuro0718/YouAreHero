#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "AttackData.h"
#include "CharacterColliderData.h"
#include "HitStopManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CharacterColliderData::CharacterColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, CharacterData* _data)
	: ColliderData		 (ColliderData::Kind::CHARACTER_CAPSULE, _priority, _tag)
	, radius			 (0.0f)
	, topPositon		 (VGet(0.0f, 0.0f, 0.0f))
	, data				 (_data)
	, isUseCollWithChara (false)
	, isUseCollWithGround(true)
	, isSetTopPosition	 (false)
	, type				 (CharaType::NONE)
{
	//�����Ȃ�
}

/// <summary>
/// �L�����N�^�[�f�[�^�̎擾
/// </summary>
const CharacterData& CharacterColliderData::GetCharacterData()const
{
	return *data;
}

/// <summary>
/// �q�b�g����
/// </summary>
void CharacterColliderData::OnHit(const AttackData& _data, const VECTOR _attackPosition)
{
	/*�J�[�h���������Ă��邩*/
	bool isGuard = true;
	if (this->data->isGuard)
	{
		this->data->stamina -= _data.blockStaminaConsumption;
		//�c��̃X�^�~�i��0�����Ȃ�K�[�h���s
		if (this->data->stamina < 0)
		{
			this->data->stamina = 0;
			this->data->isGuard = false;
			isGuard = false;
		}
	}
	else
	{
		isGuard = false;
	}

	/*�K�[�h�����G�t���O�������Ă��Ȃ�������*/
	if (!this->data->isInvinvible && !isGuard)
	{
		//�_���[�W�v�Z
		this->data->damage		 = _data.damage / this->data->defensivePower;
		//HP�����炷
		this->data->hp			-= this->data->damage;
		//�q�b�g�X�g�b�v�̐ݒ�
		this->data->hitStopTime  = _data.hitStopTime;
		this->data->hitStopType  = _data.hitStopType;
		this->data->hitStopDelay = _data.hitStopDelay;
		this->data->slowFactor	 = _data.slowFactor;
		this->data->reactionType = _data.reactionType;
	}

	/*�q�b�g�t���O�𗧂Ă�*/
	this->data->isHit = true;
}

