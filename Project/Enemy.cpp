#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "Animation.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Enemy::Enemy()
	: Character			()
	, moveTarget		(Gori::ORIGIN)
	, positionForLockon (Gori::ORIGIN)
	, animationPlayTime	(0.0f)
	, nowAnimation		(0)
	, bossState			(-1)
	, attackCount		(0)
	, angryValue		(0.0f)
	, tiredValue		(0.0f)
	, tiredDuration		(0)
{
	this->modelHandle;
	/*�R���C�_�[�f�[�^�̍쐬*/
	//this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData());
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Enemy::~Enemy()
{
}

/// <summary>
/// �A�j���[�V�����Đ����Ԃ̎擾
/// </summary>
const float Enemy::GetAnimationPlayTime()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	return json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

/// <summary>
/// ��]���̐ݒ�
/// </summary>
void Enemy::SetRotation(const VECTOR _rotation)
{
	this->collider->rigidbody.SetRotation(_rotation);
}

/// <summary>
/// �ړ��x�N�g���̐ݒ�
/// </summary>
void Enemy::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}

/// <summary>
/// ��]���̍X�V
/// </summary>
void Enemy::UpdateRotation(VECTOR _toTarget)
{
	/*�J�����̌����ƃX�e�B�b�N�̓��͂����Ƃɉ�]�����o��*/
	this->nextRotation.y = static_cast<float>(atan2(static_cast<double>(_toTarget.x), static_cast<double>(_toTarget.z)));

	/*���݂̉�]�������[�v�ŕ⊮���ďo��*/
	VECTOR nowRotation = this->collider->rigidbody.GetRotation();
	this->collider->rigidbody.SetRotation(Gori::LerpAngle(nowRotation, this->nextRotation, this->ROTATE_LERP_VALUE));
}

/// <summary>
/// �ړ����x�̍X�V
/// </summary>
void Enemy::UpdateSpeed(const float _maxSpeed, const float _accel, const float _decel)
{
	/*�V�������x���o��*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (_maxSpeed != 0)
	{
		this->speed += _accel;
		//�ő呬�x�𒴂��Ȃ��悤�ɒ�������
		if (this->speed >= _maxSpeed || this->speed <= _maxSpeed)
		{
			this->speed = _maxSpeed;
		}
	}
	else
	{
		this->speed += _decel;
		//�O�ȉ��ɂȂ�Ȃ��悤�ɒ�������
		if (this->speed <= 0)
		{
			this->speed = 0;
		}
	}
}

/// <summary>
/// �ړ��x�N�g���̍X�V
/// </summary>
void Enemy::UpdateVelocity(const bool _isLerp)
{
	/*�ړ�����������o��*/
	VECTOR rotation = this->collider->rigidbody.GetRotation();
	VECTOR direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
	direction = VNorm(direction);

	/*�V�����ړ��x�N�g�����o��*/
	VECTOR ainVelocity = VScale(direction, this->speed);
	VECTOR prevVelcity = this->collider->rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(ainVelocity.x, prevVelcity.y, ainVelocity.z);

	/*�␳�t���O�������Ă�����␳����*/
	if (_isLerp)
	{
		auto& json = Singleton<JsonManager>::GetInstance();
		newVelocity = Gori::LerpVECTOR(prevVelcity, ainVelocity, this->VELOCITY_LERP_VALUE);
	}
	
	this->collider->rigidbody.SetVelocity(newVelocity);
}

/// <summary>
/// �ړ�����
/// </summary>
void Enemy::Move(const float _maxSpeed, const float _accel, const float _decel, const bool _isLerp)
{
	/*��]���̍X�V*/
	//�ړ��ڕW�̐ݒ�
	auto& player = Singleton<PlayerManager>::GetInstance();
	this->moveTarget = player.GetRigidbody().GetPosition();
	//�ڕW�܂ł̃x�N�g�����o��
	VECTOR toTarget = VSub(this->collider->rigidbody.GetPosition(), this->moveTarget);
	//��]���̍X�V
	UpdateRotation(toTarget);

	/*�ړ����x�̍X�V*/
	UpdateSpeed(_maxSpeed, _accel, _decel);

	/*�ړ��x�N�g�����o��*/
	UpdateVelocity(_isLerp);
}

/// <summary>
/// �F�̕ύX
/// </summary>
void Enemy::ChangeAngryColor()
{
	//MV1SetEmiColorScale(this->modelHandle, this->ANGRY_COLOR);
	//MV1SetAmbColorScale(this->modelHandle, this->ANGRY_COLOR);
	MV1SetDifColorScale(this->modelHandle, this->angryColor);
}
void Enemy::ChangeTiredColor()
{
	//MV1SetEmiColorScale(this->modelHandle, this->TIRED_COLOR);
	MV1SetDifColorScale(this->modelHandle, this->tiredColor);
}
void Enemy::ChangeNormalColor()
{
	//MV1SetEmiColorScale(this->modelHandle, this->normalEmiColor);
	//MV1SetAmbColorScale(this->modelHandle, this->normalAmbColor);
	MV1SetDifColorScale(this->modelHandle, this->normalColor);
}

void Enemy::OffIsHit()
{
	this->collider->data->isHit = false;
}