#include <DxLib.h>
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Character::Character()
	: modelHandle	(-1)
	, animation		(nullptr)
	, state			(nullptr)
	, collider		(nullptr)
	, speed			(0.0f)
	, isAlive		(false)
	, isGround		(false)
	, entryInterval	(0)
{
	this->animation = new Animation();
	this->state = new BitFlag();
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
/// ���`�⊮
/// </summary>
float Character::Lerp(const float _start, const float _end, const float _percent)
{
	return _start + _percent * (_end - _start);
}
VECTOR Character::Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
{
	VECTOR out = { 0.0f,0.0f,0.0f };
	out.x = Lerp(_start.x, _end.x, _percent.x);
	out.y = Lerp(_start.y, _end.y, _percent.y);
	out.z = Lerp(_start.z, _end.z, _percent.z);
	return out;
}

/// <summary>
/// �_���[�W�f�[�^�̎擾
/// </summary>
const CharacterData& Character::GetCharacterData()const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	return *collider.data;
}

const int Character::GetHP()const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	return collider.data->hp;
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