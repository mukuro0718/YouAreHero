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
#include "BossSlash2Attack.h"
#include "BossSlash2Action.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossSlash2Action::BossSlash2Action()
	: isClose			(false)
	, ATTACK_TYPE		(static_cast<int>(Boss::AttackType::SLASH_2))
	, HIT_STOP_DELAY	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_DELAY"][this->ATTACK_TYPE])
	, HIT_STOP_TYPE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_HIT_STOP_TIME"][this->ATTACK_TYPE])
	, HIT_STOP_TIME		(static_cast<int>(HitStop::Type::STOP))
	, SLOW_FACTOR		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["OFFENSE_SLOW_FACTOR"][this->ATTACK_TYPE])
	, ROTATE_FIX_COUNT	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATION_FIX_COUNT"])
	, ROTATE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_ROTATE_PLAY_TIME"])
	, MOVE_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_MOVE_PLAY_TIME"])
	, STOP_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_STOP_PLAY_TIME"])
	, STOP_DISTANCE		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_STOP_MOVE_DISTANCE"])
	, ROTATE_LERP_VALUE	(Gori::Convert(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ROTATE_LERP_VALUE"]))
	, MAX_INTERVAL		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_INTERVAL"])
	, SLOW_PLAY_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_SLOW_PLAY_TIME"])
	, ACTION_DISTANCE	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["ACTION_DISTANCE"][this->ATTACK_TYPE])
	, SLOW_LIMIT_TIME	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["SLASH_2_SLOW_LIMIT_TIME"])
	, CHECK_STATE		(static_cast<int>(Boss::BossState::NORMAL))
{
	this->attack			= new BossSlash2Attack(static_cast<int>(BossAttack::AttackType::SLASH_2));
	auto& json	= Singleton<JsonManager>::GetInstance();
	this->speed				= json.GetJson(JsonManager::FileType::ENEMY)["SLASH_2_MOVE_SPEED"];
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::SLASH_2);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossSlash2Action::~BossSlash2Action()
{

}

/// <summary>
/// ������
/// </summary>
void BossSlash2Action::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isClose				 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->nowTotalAnimPlayTime	 = 0.0f;
	this->attack->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void BossSlash2Action::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() < 0) { this->isSelect = false; return; }


	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();

	/*�U������*/
	{
		//����������Ă��Ȃ�������
		if (!this->isInitialize)
		{
			//�U���^�C�v�̐ݒ�
			_boss.SetAttackType(Boss::AttackType::SLASH_2);
			//�U���t���O�𗧂Ă�
			this->attack->OnIsStart();
			//�ړ��ڕW�̐ݒ�
			this->moveTarget = player.GetRigidbody().GetPosition();
			//�������t���O�𗧂Ă�
			this->isInitialize = true;
			//�A�j���[�V�����̐ݒ�
			_boss.SetNowAnimation(this->nextAnimation);
			//�A�j���[�V�����Đ�����
			_boss.SetAnimationPlayTime(this->animationPlayTime);
		}
	}

	/*�q�b�g�X�g�b�v*/
	{
		//�q�b�g�X�g�b�v�̍X�V
		if (this->attack->GetIsHitAttack())
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_HEAVY_ATTACK);
			//�q�b�g�X�g�b�v�̐ݒ�
			_boss.SetHitStop(this->HIT_STOP_TIME, this->HIT_STOP_TYPE, this->HIT_STOP_DELAY, this->SLOW_FACTOR);
			//�U���q�b�g�t���O������
			this->attack->OffIsHitAttack();
		}
	}

	/*�t���[���J�E���g����]�萔�ȏゾ�����狖�t���O�𗧂Ă�*/
	this->frameCount++;
	if (this->frameCount > this->ROTATE_FIX_COUNT)
	{
		this->isAllowAction = true;
	}

	/*�ړ�����*/		
	VECTOR position		= _boss.GetRigidbody().GetPosition();	//���W
	VECTOR nowRotation	= _boss.GetRigidbody().GetRotation();	//��]��
	VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);			//����
	float  speed		= 0.0f;									//�ړ��̑���
	bool   isRotation	= false;								//��]���邩
	bool   isMove		= false;								//�ړ����邩
	//�Đ����Ԃ��萔�͈͈ȓ��Ȃ�ړ��t���O�𗧂Ă�
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
	//��]�t���O�������Ă�����
	if (isRotation)
	{
		//���W�ƈړ��ڕW�Ԃ̃x�N�g��
		VECTOR positonToTargetVector = VSub(position, this->moveTarget);
		//��]����⊮����
		nowRotation = GetLerpRotation(_boss, positonToTargetVector, nowRotation, this->ROTATE_LERP_VALUE);
		//��]������
		_boss.SetRotation(nowRotation);
	}
	//��x�ł��߂Â��Ă��Ȃ������ړ��t���O�������Ă�����
	if (!this->isClose && isMove)
	{
		//���W�ƈړ��ڕW�Ԃ̃x�N�g��
		VECTOR positonToTargetVector = VSub(position, this->moveTarget);
		//���W�ƈړ��ڕW�Ƃ̋��������߂�
		const float DISTANCE = VSize(positonToTargetVector);
		//�������萔�ȏゾ������ړ����x��������
		if (DISTANCE >= this->STOP_DISTANCE)
		{
			speed = this->speed;
		}
		//������������������t���O�𗧂Ă�
		else
		{
			this->isClose = true;
		}
	}
	//�ړ��ڕW�̐ݒ�
	_boss.SetNowMoveTarget(this->moveTarget);
	//��]�������ƂɈړ��x�N�g�����o��
	direction = VGet(-sinf(nowRotation.y), 0.0f, -cosf(nowRotation.y));
	//�ړ��x�N�g���𐳋K��
	direction = VNorm(direction);
	//�V�����ړ��x�N�g�����o���i�d�͂��l�����āAY�����̂ݑO�̂��̂��g�p����j
	VECTOR aimVelocity = VScale(direction, speed);					 //�Z�o���ꂽ�ړ��x�N�g��
	VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				 //�O�̈ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��
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
void BossSlash2Action::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;

	/*�C���^�[�o�����c���Ă����瑁�����^�[��*/
	if (this->interval != 0)
	{
		this->interval--;
		return;
	}

	/*��Ԃ�NORMAL��������~���l�𑝉�����*/
	if (_boss.GetBossState() == this->CHECK_STATE)
	{
		/*���������߂�*/
		auto& player = Singleton<PlayerManager>::GetInstance();
		VECTOR position		= _boss.GetRigidbody().GetPosition();	//���W
		VECTOR moveTarget	= player.GetRigidbody().GetPosition();	//�ړ��ڕW
		VECTOR toTarget		= VSub(position, moveTarget);			//�ڕW���猻�݂̈ړ��ڕW�ւ̃x�N�g��
		float  distance		= VSize(toTarget);						//����

		/*�����{�X�ƃv���C���[�̊Ԃ��萔�ȓ��Ȃ�~���l��{��������*/
		if (distance <= this->ACTION_DISTANCE)
		{
			//�R���{�����c���Ă��Ȃ������烊�^�[��
			if (_boss.GetAttackCount() == 0)return;
			this->parameter->desireValue = this->maxDesireValue;
		}
	}
}