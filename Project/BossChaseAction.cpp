#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossChaseAction.h"
#include "PlayerManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossChaseAction::BossChaseAction()
{
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->rotateLerpValue	= Gori::Convert(json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]);
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	this->moveDistance		= json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"];
	this->maxInterval		= json.GetJson(JsonManager::FileType::ENEMY)["CHASE_INTERVAL"];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::WALK);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossChaseAction::~BossChaseAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossChaseAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossChaseAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	//�Ԋu�ő�����炷
	if (this->frameCount == 25)
	{
		this->frameCount = 0;
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_FOOTSTEPS);
	}
	this->frameCount++;

	if (this->parameter->desireValue != 0)
	{
		this->parameter->desireValue = 0;							//�~���l���O�ɂ���
		_boss.SetSpeed(this->speed);							//�����̐ݒ�
		_boss.SetAnimationPlayTime(this->animationPlayTime);//�A�j���[�V�����Đ����Ԃ̐ݒ�
		_boss.SetNowAnimation(this->nextAnimation);			//�A�j���[�V�����̐ݒ�
	}

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�g�p����l�̏���*/
	const VECTOR POSITION = _boss.GetRigidbody().GetPosition();	//���W
	this->moveTarget	  = player.GetRigidbody().GetPosition();//�ړ��ڕW
	VECTOR nowRotation	  = _boss.GetRigidbody().GetRotation();	//��]��
	VECTOR direction	  = VGet(0.0f, 0.0f, 0.0f);		//����

	/*�ړ��x�N�g���̐ݒ�*/
	_boss.SetNowMoveTarget(this->moveTarget);

	/*�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��*/
	VECTOR positionToTargetVector = VSub(POSITION, this->moveTarget);

	/*�A�[�N�^���W�F���g���g���Ċp�x�����߂�*/
	nowRotation = GetLerpRotation(_boss, positionToTargetVector, nowRotation, this->rotateLerpValue);

	/*��]������*/
	_boss.SetRotation(nowRotation);

	/*��]�������ƂɁA�ړ�����������o��*/
	direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));

	/*�����x�N�g���𐳋K��*/
	direction = VNorm(direction);

	/*�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j*/
	VECTOR aimVelocity = VScale(direction, this->speed);					//�Z�o���ꂽ�ړ��x�N�g��
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	_boss.PlayAnimation();

	/*�ړ��ڕW�Ƃ̋������萔�ȉ���������I���t���O������*/
	float distance = VSize(positionToTargetVector);
	if (distance <= this->moveDistance)
	{
		OffIsSelect(this->maxInterval);
	}
}
/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossChaseAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*�C���^�[�o�����c���Ă����瑁�����^�[��*/
	if (this->interval != 0)
	{
		this->interval--;
		return;
	}

	/*���������߂�*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//���W
	const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//�ړ��ڕW
	const VECTOR POSITION_TO_TARGET	= VSub(POSITION, MOVE_TARGET);			//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
	const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//����

	/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȏ㗣��Ă�����~���l��{��������*/
	if (DISTANCE >= this->moveDistance)
	{
		if (_boss.GetAttackCount() == 0)
		{
			this->parameter->desireValue = 0;
		}
		else
		{
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}