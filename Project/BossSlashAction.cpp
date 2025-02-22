#include <DxLib.h>
#include "EffekseerForDXLib.h"
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
#include "HitStop.h"
#include "BossAttack.h"
#include "BossSlashAttack.h"
#include "BossSlashAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossSlashAction::BossSlashAction()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::SLASH_1))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_FIX_COUNT	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_ROTATE_PLAY_TIME"])
	, MOVE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_MOVE_PLAY_TIME"])
	, STOP_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_STOP_PLAY_TIME"])
	, STOP_DISTANCE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_STOP_MOVE_DISTANCE"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_INTERVAL"])
	, TARGET_OFFSET		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_TARGET_OFFSET"])
	, ACTION_DISTANCE	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][this->ATTACK_TYPE])
	, CHECK_STATE		(static_cast<int>(Boss::BossState::NORMAL))
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->attack			= new BossSlashAttack(static_cast<int>(BossAttack::AttackType::SLASH_1));
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::SLASH_1);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossSlashAction::~BossSlashAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossSlashAction::Initialize()
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
void BossSlashAction::Update(Boss& _boss)
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
		//�ړ��ڕW�̐ݒ�
		const VECTOR TARGET					= player.GetRigidbody().GetPosition();									//�ڕW
		const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();									//���W
			  VECTOR positonToTargetVector	= VSub(POSITION, TARGET);											//���W�ƈړ��ڕW�Ԃ̃x�N�g��
			  VECTOR targetOffset			= VNorm(VCross(VNorm(positonToTargetVector), Gori::UP_VEC));//�ڕW�̃I�t�Z�b�g
		this->moveTarget = VAdd(TARGET, VScale(targetOffset, this->TARGET_OFFSET));
		//�������t���O�𗧂Ă�
		this->isInitialize = true;
	}

	/*�q�b�g�X�g�b�v*/
	//�U�����������Ă�����
	if (this->attack->GetIsHitAttack())
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_LIGHT_ATTACK);
		//�q�b�g�X�g�b�v�̐ݒ�
		_boss.SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
		//�U���q�b�g�t���O������
		this->attack->OffIsHitAttack();
	}

	/*�t���[���J�E���g����]�萔�ȏゾ�����狖�t���O�𗧂Ă�*/
	this->frameCount++;
	if (this->frameCount > this->ROTATE_FIX_COUNT)
	{
		this->isAllowAction = true;
	}

	/*�ړ�����*/
	const VECTOR POSITION				= _boss.GetRigidbody().GetPosition();		//���W
		  VECTOR nowRotation			= _boss.GetRigidbody().GetRotation();		//��]��
		  VECTOR positonToTargetVector	= VSub(POSITION, this->moveTarget);	//���W�ƈړ��ڕW�Ԃ̃x�N�g��
		  float  speed					= 0.0f;										//�ړ��X�s�[�h�̐ݒ�
		  bool   isRotation				= false;									//��]���邩
		  bool   isMove					= false;									//�ړ����邩
	//�Đ����Ԃ��萔�͈͓��Ȃ�ړ��t���O�𗧂Ă�
	if (this->nowTotalAnimPlayTime < this->MOVE_PLAY_TIME)
	{
		isMove = true;
	}
	//�Đ����Ԃ��萔�ȓ��Ȃ��]�t���O�𗧂Ă�
	if (this->nowTotalAnimPlayTime < this->ROTATE_PLAY_TIME)
	{
		isRotation = true;
		this->moveTarget = player.GetRigidbody().GetPosition();
	}
	//�t���O�������Ă������]
	if (isRotation)
	{
		//��]����⊮����
		nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, this->ROTATE_LERP_VALUE);
		//��]����ݒ�
		_boss.SetRotation(nowRotation);
	}
	//�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
	if (!this->isAllowAction)return;
	//��x����苗���ɋ߂Â��Ă��Ȃ� && �ړ��t���O�������Ă�����
	if (!this->isClose && isMove)
	{
		//���W�ƈړ��ڕW�Ƃ̋��������߂�
		const float DISTANCE = VSize(positonToTargetVector);
		//�������萔�ȏ�Ȃ瑬�x��ݒ肷��
		if (DISTANCE >= this->STOP_DISTANCE)
		{
			speed = this->speed;
		}
		//��薢���Ȃ�t���O�𗧂Ă�
		else
		{
			this->isClose = true;
		}
	}
	//�ړ��x�N�g���̐ݒ�
	_boss.SetNowMoveTarget(this->moveTarget);
	//�V�����ړ��x�N�g���̎Z�o
	VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
	//�ړ��x�N�g���̐ݒ�
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V��������*/
	//�A�j���[�V�����̐ݒ�
	this->nowTotalAnimPlayTime += this->animationPlayTime;
	_boss.SetNowAnimation(this->nextAnimation);
	//�Đ����Ԃ̐ݒ�
	_boss.SetAnimationPlayTime(this->animationPlayTime);
	//�A�j���[�V�����̍Đ�
	_boss.PlayAnimation();

	/*�U������*/
	this->attack->Update(this->nowTotalAnimPlayTime);

	/*�I������*/
		//�A�j���[�V�������I�����Ă�����
	if (_boss.GetIsChangeAnimation())
	{
		//������/�߂Â������𔻒肷��t���O������
		this->isInitialize			= false;
		this->isClose				= false;
		this->nowTotalAnimPlayTime	= 0.0f;
		//���̑��ϐ��̏������ƃC���^�[�o���̃Z�b�g
		OffIsSelect(this->MAX_INTERVAL);
		//�R���{�������炷
		_boss.DecAttackComboCount();
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossSlashAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*�C���^�[�o�����c���Ă����瑁�����^�[��*/
	if (this->interval != 0)
	{
		this->interval--;
		return;
	}

	/*��Ԃ�TIRED,NORMAL,ANGRY��������~���l�𑝉�����*/
	if (_boss.GetBossState() == this->CHECK_STATE)
	{
		/*���������߂�*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();		//���W
		const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();		//�ړ��ڕW
		const VECTOR TO_TARGET	 = VSub(POSITION, MOVE_TARGET);				//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
		const float  DISTANCE	 = VSize(TO_TARGET);						//����

		/*�������萔�ȓ���������~���l��ʏ�ɂ���*/
		if (DISTANCE <= this->ACTION_DISTANCE)
		{
			//�R���{���O�������瑁�����^�[��
			if (_boss.GetAttackCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}