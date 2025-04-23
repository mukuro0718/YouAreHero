#include <DxLib.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"
#include "HitStop.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Character::Character()
	: modelHandle	(-1)
	, animation		(nullptr)
	, state			(nullptr)
	, collider		(nullptr)
	, hitStop		(nullptr)
	, nextRotation	(Gori::ORIGIN)
	, spownPosition	(Gori::ORIGIN)
	, isAlive		(false)
	, isDraw		(false)
	, speed			(1.0f)
	, entryInterval	(0)
{
	this->animation = new Animation();
	this->state		= new BitFlag();
	this->hitStop	= new HitStop();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Character::~Character()
{
	DeleteMemberInstance(this->animation);
	DeleteMemberInstance(this->state);
	DeleteMemberInstance(this->collider);
	MV1DeleteModel(this->modelHandle);
}

/// <summary>
/// �`��
/// </summary>
const void Character::Draw() const
{
	/*�L�����N�^�[���̕\��*/
	DrawCharacterInfo();

	/*���f���̕`��*/
	if (this->isDraw)
	{
		MV1SetPosition(this->modelHandle, this->collider->rigidbody.GetPosition());
		MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
		MV1SetScale(this->modelHandle, this->collider->rigidbody.GetScale());
		MV1DrawModel(this->modelHandle);
	}
}

/// <summary>
/// ���[�v
/// </summary>
float Character::Lerp(const float _start, const float _end, const float _percent)
{
	return _start + _percent * (_end - _start);
}
VECTOR Character::Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
{
	VECTOR out = (Gori::ORIGIN);
	out.x = Lerp(_start.x, _end.x, _percent.x);
	out.y = Lerp(_start.y, _end.y, _percent.y);
	out.z = Lerp(_start.z, _end.z, _percent.z);
	return out;
}

/// <summary>
/// atan2�ŏo�����l��360�x�ɒ����⊮���Ȃ����]������
/// </summary>
VECTOR Character::Lerp360Angle(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
{
	vector<float> startBase = { _start.x,_start.y,_start.z };
	vector<float> endBase = { _end.x,_end.y,_end.z };
	vector<float> t = { _percent.x,_percent.y,_percent.z };
	vector<float> out;
	const float PI_2 = (2.0f * DX_PI_F);

	for (int i = 0; i < startBase.size(); i++)
	{
		/*�J�n�n�_�ƏI���n�_�����߂�*/
		float start = startBase[i];
		float end = endBase[i];
		//�I���n�_���}�C�i�X��������2�΂𑫂�
		if (end < 0.0f) end += PI_2;

		/*�J�n�n�_�ƏI���n�_�̍������߂�*/
		float diff = start - end;
		//���������}�C�i�X��������-1��������
		if (diff < 0.0f) diff *= -1.0f;

		/*�]������߂�*/
		float excess = PI_2 - diff;

		/*���Ɨ]������߂āA���̂ق����傫�����Base�̂ق��Ōv�Z*/
		if (diff > excess)
		{
			//�X�^�[�g�x�[�X��360�x�ɕϊ�����Ă���̂ŁA�� ~ -�΂͈̔͂ɖ߂�
			if (startBase[i] >= DX_PI_F)
			{
				startBase[i] -= PI_2;
			}
			out.emplace_back(Lerp(startBase[i], endBase[i], t[i]));
		}

		/*�]��̂ق����傫����΂��̂܂܌v�Z*/
		else
		{
			out.emplace_back(Lerp(start, end, t[i]));
		}

		/*360�x�͈̔͂𒴂��Ȃ��悤�ɂ���*/
		if (out[i] <= 0.0f)
		{
			out[i] += PI_2;
		}
		else if (out[i] >= PI_2)
		{
			out[i] -= PI_2;
		}
	}

	VECTOR rotation = VGet(0.0f, out[1], 0.0f);
	return rotation;
}

/// <summary>
/// �L�����N�^�[�f�[�^�̎擾
/// </summary>
const CharacterData& Character::GetCharacterData()const
{
	return *this->collider->data;
}

/// <summary>
/// �R���C�_�[�f�[�^�̎擾
/// </summary>
const ColliderData& Character::GetColliderData()const
{
	return *this->collider;
}

const int Character::GetHP()const
{
	return this->collider->data->hp;
}

/// <summary>
/// ���W�b�h�{�f�B�̎擾
/// </summary>
const Rigidbody& Character::GetRigidbody()const
{
	return this->collider->rigidbody;
}

/// <summary>
/// �A�j���[�V�����ύX�t���O�̎擾
/// </summary>
const bool Character::GetIsChangeAnimation()const
{
	return this->animation->GetIsChangeAnim();
}


void Character::OffIsAlive()
{ 
	this->isAlive = false;
	this->collider->isUseCollWithChara = false;
}

/// <summary>
/// ��]���̃Z�b�g
/// </summary>
void Character::SetRotation(const VECTOR _now, const VECTOR _next)
{
	this->nextRotation = _next;
	this->collider->rigidbody.SetRotation(_now);
}

/// <summary>
/// �x���V�e�B�̐ݒ�
/// </summary>
void Character::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}

/// <summary>
/// �X�e�[�W�O�ɏo���璆���ɖ߂�
/// </summary>
void Character::RespawnIfOutOfStage()
{
	this->collider->rigidbody.SetPosition(this->spownPosition);
	//this->collider->data->hp = 0;
}

/// <summary>
/// �A�N�V�����N���X���ŁA�q�b�g�X�g�b�v��ݒ肷�闭�߂̂���
/// </summary>
void Character::SetHitStop(const int _time, const int _type, const int _delay, const float _factor)
{
	this->hitStop->SetHitStop(static_cast<float>(_time), _type, _delay, _factor);
}
