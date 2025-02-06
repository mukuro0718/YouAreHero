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
#include "BossStabAttack.h"
#include "BossStabAction.h"
#include "PlayerManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossStabAction::BossStabAction()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::STAB))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_FIX_COUNT	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_ROTATE_PLAY_TIME"])
	, MOVE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_PLAY_TIME"])
	, STOP_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_PLAY_TIME"])
	, STOP_DISTANCE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_STOP_MOVE_DISTANCE"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_INTERVAL"])
	, SLOW_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_PLAY_TIME"])
	, MIN_ACTION_DISTANCE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["NEAR_DISTANCE"])
	, MAX_ACTION_DISTANCE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["MIDDLE_DISTANCE"])
	, SLOW_LIMIT_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_LIMIT_TIME"])
	, CHECK_STATE	(static_cast<int>(Boss::BossState::ANGRY))
{
	this->attack			= new BossStabAttack(static_cast<int>(BossAttack::AttackType::STAB));
	auto& json				= Singleton<JsonManager>::GetInstance();
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["STAB_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::STAB);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossStabAction::~BossStabAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossStabAction::Initialize()
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
	this->hitStop->Initialize();
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossStabAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	
	/*����������Ă��Ȃ������珉����*/
	if (!this->isInitialize)
	{
		//�U���^�C�v�̐ݒ�
		_boss.SetAttackType(Boss::AttackType::STAB);
		//�U���t���O�𗧂Ă�
		this->attack->OnIsStart();
		//�������t���O�𗧂Ă�
		this->isInitialize = true;
		//�A�j���[�V�����̐ݒ�
		_boss.SetNowAnimation(this->nextAnimation);
	}

	/*�q�b�g�X�g�b�v�̍X�V*/
	//�U�����������Ă�����
	if (this->attack->GetIsHitAttack())
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
		//�q�b�g�X�g�b�v�̐ݒ�
		this->hitStop->SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
		//�U���q�b�g�t���O������
		this->attack->OffIsHitAttack();
	}
	//�q�b�g�X�g�b�v���������瑁�����^�[��
	if (this->hitStop->IsHitStop()) return;

	/*�t���[���J�E���g����]�萔�ȏゾ�����狖�t���O�𗧂Ă�*/
	this->frameCount++;
	if (this->frameCount > this->ROTATE_FIX_COUNT)
	{
		this->isAllowAction = true;
	}
	else
	{
		this->moveTarget = player.GetRigidbody().GetPosition();
	}

	/*���݂̃A�j���[�V�����Đ����Ԃɂ���ăt���O�����߂�*/
	bool isRotation = false;	//��]���邩
	bool isMove		= false;	//�ړ����邩
	//�Đ����Ԃ��萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
	if (this->nowTotalAnimPlayTime < this->MOVE_PLAY_TIME)
	{
		isMove = true;
	}
	//�Đ����Ԃ��萔�ȓ��Ȃ�ړ��t���O�𗧂Ă�
	else if (this->nowTotalAnimPlayTime >= this->STOP_PLAY_TIME)
	{
		isMove = false;
	}
	//�Đ����Ԃ��萔�ȓ��Ȃ��]�t���O�𗧂Ă�
	if (this->nowTotalAnimPlayTime < this->ROTATE_PLAY_TIME)
	{
		isRotation = true;
	}

	/*�ړ�����*/
	//�g�p����l�̏���
	const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//���W
		  VECTOR nowRotation = _boss.GetRigidbody().GetRotation();	//��]��
		  VECTOR toTarget	 = VSub(POSITION, this->moveTarget);	//���W�ƈړ��ڕW�Ԃ̃x�N�g��
		  VECTOR direction	 = VGet(0.0f, 0.0f, 0.0f);				//����
		  float  speed		 = 0.0f;								//�ړ��X�s�[�h�̐ݒ�
	//��]����
	if (isRotation)
	{
		//��]����⊮����
		nowRotation = GetLerpRotation(_boss, toTarget, nowRotation, this->ROTATE_LERP_VALUE);
		//��]����ݒ�
		_boss.SetRotation(nowRotation);
	}
	//�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
	if (!this->isAllowAction)return;
	//�ړ����x�̐ݒ�
	//��x�ł��߂Â��Ă��Ȃ������ړ��t���O�������Ă��Ȃ�������
	if (!this->isClose && isMove)
	{
		//���W�ƈړ��ڕW�Ƃ̋��������߂�
		const float DISTANCE = VSize(toTarget);
		//�������萔�ȏォ
		if (DISTANCE >= this->STOP_DISTANCE)
		{
			speed = this->speed;
		}
		else
		{
			//�߂Â����t���O�𗧂Ă�
			this->isClose = true;
		}
	}
	//�ړ��x�N�g���̐ݒ�
	//�ړ��ڕW�̐ݒ�
	_boss.SetNowMoveTarget(this->moveTarget);
	//�V�����ړ��x�N�g���̎Z�o
	VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
	//�ړ��x�N�g���̐ݒ�
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V��������*/
	float playTime = 0.0f;
	if (this->nowTotalAnimPlayTime <= this->SLOW_LIMIT_TIME)
	{
		playTime = this->SLOW_PLAY_TIME;
	}
	else
	{
		playTime = this->animationPlayTime;
	}
	this->nowTotalAnimPlayTime += playTime;
	_boss.SetAnimationPlayTime(playTime);
	//�A�j���[�V�����̍Đ�
	_boss.PlayAnimation();

	/*�U������̍X�V*/
	this->attack->Update(this->nowTotalAnimPlayTime);

	/*�I������*/
	//�A�j���[�V�������I�����Ă�����
	if (_boss.GetIsChangeAnimation())
	{
		//�e�t���O������
		this->isInitialize = false;
		this->isClose = false;
		this->nowTotalAnimPlayTime = 0.0f;
		//���̑��ϐ��̏������ƃC���^�[�o���̃Z�b�g
		OffIsSelect(this->MAX_INTERVAL);
		//�R���{�������炷
		_boss.DecAttackComboCount();
	}
}
/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossStabAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*��Ԃ�ANGRY��������~���l�𑝉�����*/
	if (_boss.GetAngryState() == this->CHECK_STATE)
	{
		/*���������߂�*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();	//���W
		const VECTOR MOVE_TARGET		= player.GetRigidbody().GetPosition();	//�ړ��ڕW
		const VECTOR POSITION_TO_TARGET = VSub(POSITION, MOVE_TARGET);			//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
		const float  DISTANCE			= VSize(POSITION_TO_TARGET);			//����
		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (this->MIN_ACTION_DISTANCE <= DISTANCE && DISTANCE <= this->MAX_ACTION_DISTANCE)
		{
			//�R���{�����c���Ă�����
			if (_boss.GetAttackComboCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}