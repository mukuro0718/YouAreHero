#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "VECTORtoUseful.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "HitStop.h"
#include "BossAttack.h"
#include "BossRotateSlashAttack.h"
#include "BossRotateSlashAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRotateSlashAction::BossRotateSlashAction()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::ROTATE_SLASH))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_SLASH_ROTATE_PLAY_TIME"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_SLASH_INTERVAL"])
	, ACTION_DISTANCE	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][this->ATTACK_TYPE])
	, CHECK_STATE		(static_cast<int>(Boss::BossState::ANGRY))

{
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->attack			= new BossRotateSlashAttack(ATTACK_TYPE);
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::ROTATE_SLASH);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRotateSlashAction::~BossRotateSlashAction()
{

}

/// <summary>
/// ������
/// </summary>
void BossRotateSlashAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->isAllowAction			 = false;
	this->frameCount			 = 0;
	this->nowTotalAnimPlayTime	 = 0.0f;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->attack->Initialize();
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRotateSlashAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*����������Ă��Ȃ������珉����*/
	if (!this->isInitialize)
	{
		//�U���^�C�v�̐ݒ�
		_boss.SetAttackType(Boss::AttackType::SLASH_1);
		//�U���t���O�𗧂Ă�
		this->attack->OnIsStart();
		//�������t���O�𗧂Ă�
		this->isInitialize = true;
	}

	/*�q�b�g�X�g�b�v�̍X�V*/
	//�U�����������Ă�����
	if (this->attack->GetIsHitAttack())
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
		//�q�b�g�X�g�b�v�̐ݒ�
		_boss.SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
		//�U���q�b�g�t���O������
		this->attack->OffIsHitAttack();
	}

	/*�ړ�����*/
	//�g�p����l�̏���
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();	//���W
	const VECTOR MOVE_TARGET			= player.GetRigidbody().GetPosition();	//�ړ��ڕW
		  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();	//��]��
		  VECTOR positonToTargetVector	= VSub(POSITION, MOVE_TARGET);	//���W�ƈړ��ڕW�Ԃ̃x�N�g��
		  VECTOR direction				= VGet(0.0f, 0.0f, 0.0f);			//����
		  float  speed					= 0.0f;									//�ړ��X�s�[�h�̐ݒ�
		  bool   isRotation				= false;								//��]���邩
	//���̍U���͍ŏ�����U������
	this->isAllowAction = true;
	//�Đ����Ԃ��萔�ȓ��Ȃ��]�t���O�𗧂Ă�
	if (this->nowTotalAnimPlayTime < this->ROTATE_PLAY_TIME)
	{
		isRotation = true;
		this->moveTarget = player.GetRigidbody().GetPosition();
	}
	//��]����
	if (isRotation)
	{
		//��]����⊮����
		nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, this->ROTATE_LERP_VALUE);
		//��]����ݒ�
		_boss.SetRotation(nowRotation);
	}
	//�ړ��x�N�g���̐ݒ�
	_boss.SetNowMoveTarget(this->moveTarget);
	//�V�����ړ��x�N�g���̎Z�o
	VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
	//�ړ��x�N�g���̐ݒ�
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V��������*/
	this->nowTotalAnimPlayTime += this->animationPlayTime;
	//�A�j���[�V�����̐ݒ�
	_boss.SetNowAnimation(this->nextAnimation);
	//�Đ����Ԃ̐ݒ�
	_boss.SetAnimationPlayTime(this->animationPlayTime);
	//�A�j���[�V�����̍Đ�
	_boss.PlayAnimation();

	/*�U������̍X�V*/
	this->attack->Update(this->nowTotalAnimPlayTime);

	/*�I������*/
	if (_boss.GetIsChangeAnimation())
	{
		this->isInitialize			= false;
		this->isClose				= false;
		this->nowTotalAnimPlayTime	= 0.0f;
		OffIsSelect(this->MAX_INTERVAL);
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRotateSlashAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*�C���^�[�o�����c���Ă����瑁�����^�[��*/
	if (this->interval != 0)
	{
		this->interval--;
		return;
	}

	/*��Ԃ�NORMAL,ANGRY��������~���l�𑝉�����*/
	if (_boss.GetBossState() == this->CHECK_STATE)
	{
		/*���������߂�*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//���W
		const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//�ړ��ڕW
		const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);			//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
		const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//����

		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (DISTANCE <= this->ACTION_DISTANCE)
		{
			//�R���{���O�������瑁�����^�[��
			if (_boss.GetAttackCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}