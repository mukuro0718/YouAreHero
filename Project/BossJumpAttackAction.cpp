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
#include "BossJumpAttack.h"
#include "BossJumpAttackAction.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossJumpAttackAction::BossJumpAttackAction()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::JUMP_ATTACK))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_FIX_COUNT	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTAC_ROTATE_PLAY_TIME"])
	, MOVE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTAC_MOVE_PLAY_TIME"])
	, STOP_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_STOP_PLAY_TIME"])
	, STOP_DISTANCE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_STOP_MOVE_DISTANCE"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_INTERVAL"])
	, MIN_ACTION_DISTANCE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["NEAR_DISTANCE"])
	, MAX_ACTION_DISTANCE(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["MIDDLE_DISTANCE"])
	, CHECK_STATE		(static_cast<int>(Boss::BossState::NORMAL))
	, TARGET_OFFSET		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["STAB_INTERVAL"])
{
	const int ATTACK_TYPE	= static_cast<int>(Boss::AttackType::JUMP_ATTACK);
	this->attack			= new BossJumpAttack(ATTACK_TYPE);
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["JUMP_ATTACK_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::SLASH_2);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossJumpAttackAction::~BossJumpAttackAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossJumpAttackAction::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
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
void BossJumpAttackAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*����������Ă��Ȃ������珉����*/
	if (!this->isInitialize)
	{
		//�U���^�C�v�̐ݒ�
		_boss.SetAttackType(Boss::AttackType::JUMP_ATTACK);
		//�U���t���O�𗧂Ă�
		this->attack->OnIsStart();
		//�ړ��ڕW�̐ݒ�
		const VECTOR TARGET				= player.GetRigidbody().GetPosition();							//�ڕW
		const VECTOR POSITION			= _boss.GetRigidbody().GetPosition();							//���W
			  VECTOR toTarget			= VSub(POSITION, TARGET);									//���W�ƈړ��ڕW�Ԃ̃x�N�g��
			  VECTOR targetOffsetVector = VNorm(VCross(VNorm(toTarget), Gori::UP_VEC));		//�ڕW�I�t�Z�b�g�x�N�g��
		this->moveTarget = VAdd(TARGET, VScale(targetOffsetVector, this->TARGET_OFFSET));
		//�������t���O�𗧂Ă�
		this->isInitialize = true;
	}

	/*�q�b�g�X�g�b�v*/
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
	//�q�b�g�X�g�b�v�����Ă����瑁�����^�[��
	if (this->hitStop->IsHitStop()) return;

	/*�t���[���J�E���g����]�萔�ȏゾ�����狖�t���O�𗧂Ă�*/
	this->frameCount++;
	if (this->frameCount > this->ROTATE_FIX_COUNT)
	{
		this->isAllowAction = true;
	}

	/*�ړ�����*/
	VECTOR position				 = _boss.GetRigidbody().GetPosition();		//���W
	VECTOR nowRotation			 = _boss.GetRigidbody().GetRotation();		//��]��
	VECTOR positonToTargetVector = VSub(position, this->moveTarget);	//���W�ƈړ��ڕW�Ԃ̃x�N�g��
	VECTOR direction			 = VGet(0.0f, 0.0f, 0.0f);				//����
	float  speed				 = 0.0f;									//�ړ��X�s�[�h�̐ݒ�
	bool   isRotation			 = false;									//��]���邩
	bool   isMove				 = false;									//�ړ����邩
	//�Đ����Ԃ��萔�͈͓���������t���O�𗧂Ă�
	if (this->nowTotalAnimPlayTime > this->MOVE_PLAY_TIME && this->nowTotalAnimPlayTime < this->STOP_PLAY_TIME)
	{
		isMove = true;
	}
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
	//�A�N�V�������t���O�������Ă��Ȃ���Α������^�[��
	if (!this->isAllowAction)return;
	//��x����苗���ɋ߂Â��Ă��Ȃ� && �ړ��t���O�������Ă�����
	if (isMove)
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
	//�ړ��ڕW�̐ݒ�
	_boss.SetNowMoveTarget(this->moveTarget);
	//�V�����ړ��x�N�g���̎Z�o
	VECTOR newVelocity = CalcVelocity(_boss.GetRigidbody().GetVelocity(), nowRotation, speed);
	//�ړ��x�N�g���̐ݒ�
	_boss.SetVelocity(newVelocity);

	/*�A�j���[�V��������*/
	//�A�j���[�V�����̐ݒ�
	_boss.SetNowAnimation(static_cast<int>(Boss::AnimationType::JUMP_ATTACK));
	//�Đ����Ԃ̐ݒ�
	float playTime = this->animationPlayTime;
	this->nowTotalAnimPlayTime += playTime;
	_boss.SetAnimationPlayTime(playTime);
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
void BossJumpAttackAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	
	/*��Ԃ�NORMAL��������~���l�𑝉�����*/
	if (_boss.GetAngryState() >= this->CHECK_STATE)
	{
		/*���������߂�*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		const VECTOR POSITION	 = _boss.GetRigidbody().GetPosition();	//���W
		const VECTOR MOVE_TARGET = player.GetRigidbody().GetPosition();	//�ړ��ڕW
		const VECTOR TO_TARGET	 = VSub(POSITION, MOVE_TARGET);	//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
		const float  DISTANCE	 = VSize(TO_TARGET);			//����

		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (this->MIN_ACTION_DISTANCE <= DISTANCE && DISTANCE <= this->MAX_ACTION_DISTANCE)
		{
			//�����R���{���O�������瑁�����^�[��
			if (_boss.GetAttackComboCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}

}
