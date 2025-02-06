#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "ActionParameter.h"
#include "BossAction.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossRoarAction.h"
#include "EffectManager.h"
#include "SoundManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRoarAction::BossRoarAction()
	: prevState				(0)
	, isFinishedFirstRoar	(false)
	, checkedState			(0)
	, PLAY_ROAR_SOUND_PLAY_TIME(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::ENEMY)["PLAY_ROAR_SOUND_PLAY_TIME"])
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->checkedState		= static_cast<int>(Boss::BossState::ANGRY);
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::ROAR);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRoarAction::~BossRoarAction()
{
}
/// <summary>
/// ������
/// </summary>
void BossRoarAction::Initialize()
{
	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isFinishedFirstRoar	 = false;
	this->isPriority			 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = this->maxDesireValue;
	this->parameter->interval	 = 0;
	this->prevState				 = 1;
	this->frameTime				 = 0;
	this->nowTotalAnimPlayTime	 = 0.0f;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRoarAction::Update(Boss& _boss)
{
	//int startTime = GetNowCount();

	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*���̏����̊J�n���Ɉ�x�����Ă�*/
	if (this->frameCount == 0)
	{
		//�{���Ԃ����킹��
		this->prevState = _boss.GetAngryState();
		//�I������Ă�����~���l���O�ɂ���
		this->parameter->desireValue = 0;
		//�A�j���[�V�����̐ݒ�
		_boss.SetNowAnimation(this->nextAnimation);
		//�A�j���[�V�����Đ����Ԃ̐ݒ�
		_boss.SetAnimationPlayTime(this->animationPlayTime);
		//�X�s�[�h���O�ɂ���
		float speed = 0.0f;
		_boss.SetSpeed(speed);
		//��]�������ƂɁA�ړ�����������o��
		VECTOR	rotation  = _boss.GetRigidbody().GetRotation();					//��]��
		VECTOR	direction = VGet(0.0f, 0.0f, 0.0f);								//����
				direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
				direction = VNorm(direction);
		//�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j
		VECTOR aimVelocity  = VScale(direction, speed);								//�Z�o���ꂽ�ړ��x�N�g��
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();					//�O�̈ړ��x�N�g��
		VECTOR newVelocity  = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);	//�V�����ړ��x�N�g��
		//�ړ��x�N�g���̐ݒ�
		_boss.SetVelocity(newVelocity);
		this->frameCount++;
	}

	/*�A�j���[�V�����̍Đ�*/
	_boss.PlayAnimation();
	
	/*�T�E���h�G�t�F�N�g�̍Đ�*/
	if (this->nowTotalAnimPlayTime < this->PLAY_ROAR_SOUND_PLAY_TIME)
	{
		this->nowTotalAnimPlayTime += this->animationPlayTime;
		if (this->nowTotalAnimPlayTime >= this->PLAY_ROAR_SOUND_PLAY_TIME)
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OnIsPlayEffect(SoundManager::EffectType::MONSTER_ROAR);
			auto& effect = Singleton<EffectManager>::GetInstance();
			effect.OnIsEffect(EffectManager::EffectType::BOSS_ROAR);
		}
	}

	/*���K���ɃA�j���[�V�������I�����Ă�����A�I���t���O�������ăt�F�[�Y�𓝈ꂷ��*/
	if (this->isSelect && _boss.GetIsChangeAnimation())
	{
		OffIsSelect(0);
		this->isFinishedFirstRoar = true;
		this->frameCount = 0;
		this->nowTotalAnimPlayTime = 0.0f;
	}
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRoarAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*����HP���O�ȉ���������~���l���O�ɂ��đ������^�[��*/
	if (_boss.GetHP() <= 0)
	{
		return;
	}

	/*��x�����K�����Ă��Ȃ���Ι��K�t���O�ƗD��t���O�𗧂Ă�*/
	if (!this->isFinishedFirstRoar)
	{
		this->parameter->desireValue = this->maxDesireValue;
		this->isPriority = true;
	}

	/*AngryStateType��ANGRY�ɂȂ�������K������*/
	int nowState = _boss.GetAngryState();
	if (nowState != this->prevState)
	{
		if (nowState == this->checkedState)
		{
			this->parameter->desireValue = this->maxDesireValue;
			this->isPriority = true;
		}
		else
		{
			this->prevState = nowState;
		}
	}
}

