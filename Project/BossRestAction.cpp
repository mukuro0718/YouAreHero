#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "ActionParameter.h"
#include "Rigidbody.h"
#include "BossAction.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossRestAction.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossRestAction::BossRestAction()
	: maxFrameCount (0)
	, nextAnimation2(0)
	, nowAnimationType(0)
	, isChangeColorScale(false)
	, prevState(0)
	, isInitializeColorScale(false)
	, isDownUp(false)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->nextAnimation		= static_cast<int>(Boss::AnimationType::DOWN);
	this->nextAnimation2	= static_cast<int>(Boss::AnimationType::DOWN_UP);
	this->maxDesireValue	= json.GetJson(JsonManager::FileType::ENEMY)["MAX_DESIRE_VALUE"];
	this->maxFrameCount		= json.GetJson(JsonManager::FileType::ENEMY)["REST_ACTION_MAX_FRAME"];
	this->checkedState		= static_cast<int>(Boss::BossState::TIRED);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nextAnimation];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossRestAction::~BossRestAction()
{

}
/// <summary>
/// ������
/// </summary>
void BossRestAction::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->isSelect				 = false;
	this->isInitialize			 = false;
	this->isInitializeColorScale = false;
	this->isChangeColorScale	 = false;
	this->isDownUp				 = false;
	this->frameCount			 = 0;
	this->parameter->desireValue = 0;
	this->parameter->interval	 = 0;
	this->prevState = 1;
	this->nowAnimationType		 = -1;
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRestAction::Update(Boss& _boss)
{
	/*���S���Ă�����isSelect��false�ɂ��đ������^�[��*/
	if (_boss.GetHP() <= 0) { this->isSelect = false; return; }

	/*�{���Ԃ����킹��*/
	if (this->frameCount == 0)
	{
		this->prevState = _boss.GetBossState();
	}

	/*�ړ�����*/
	{
		//�g�p����l�̏���
		const float  SPEED		= 0.0f;								 //�X�s�[�h
		const VECTOR ROTATION	= _boss.GetRigidbody().GetRotation();//��]��
			  VECTOR direction	= VGet(0.0f, 0.0f, 0.0f);		 //����
		//�X�s�[�h�̐ݒ�
		_boss.SetSpeed(SPEED);
		//��]�������ƂɁA�ړ�����������o��
		direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));
		//�����x�N�g���𐳋K��
		direction = VNorm(direction);
		//�ړ��x�N�g�����o���i�d�͂����Z���邽�߁AY�x�N�g���̂ݑO�̃x�N�g�����g�p����j
		VECTOR aimVelocity = VScale(direction, SPEED);					//�Z�o���ꂽ�ړ��x�N�g��
		VECTOR prevVelocity = _boss.GetRigidbody().GetVelocity();				//�O�̈ړ��x�N�g��
		VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��
		//�ړ��x�N�g���̐ݒ�
		_boss.SetVelocity(newVelocity);
	}
	
	/*�t���[���v��*/
	bool isEndCount = FrameCount(this->maxFrameCount);

	/*�A�j���[�V�����̏���*/
	{
		float animationPlayTime = _boss.GetAnimationPlayTime();
		bool isPlayAnimation = true;
		//�J�E���g���I�����Ă��Ȃ����
		if (!isEndCount)
		{
			if (_boss.GetIsChangeAnimation() && this->nowAnimationType != -1)
			{
				isPlayAnimation = false;
			}
			this->nowAnimationType = this->nextAnimation;
		}
		else
		{
			this->nowAnimationType = this->nextAnimation2;
			this->isDownUp = true;
		}
		//�A�j���[�V�����Đ��t���O�������Ă�����
		if (isPlayAnimation)
		{
			//�A�j���[�V�����̐ݒ�
			_boss.SetNowAnimation(this->nowAnimationType);
			//�Đ����Ԃ̐ݒ�
			_boss.SetAnimationPlayTime(animationPlayTime);
			//�A�j���[�V�����̍Đ�
			_boss.PlayAnimation();
		}
	}

	/*�t���[���v�����I�����Ă�����*/
	if (isEndCount && _boss.GetIsChangeAnimation())
	{
		OffIsSelect(0);
		this->isChangeColorScale = false;
		this->nowAnimationType = -1;
	}
}

/// <summary>
/// �p�����[�^�[�̌v�Z
/// </summary>
void BossRestAction::CalcParameter(const Boss& _boss)
{
	this->parameter->desireValue = 0;
	this->isPriority = false;

	/*����HP���O�ȉ���������~���l���O�ɂ��ėD��t���O������*/
	if (_boss.GetHP() <= 0)return;

	/*AngryState��TIRED�̎���*/
	int nowState = _boss.GetBossState();
	if (nowState == this->checkedState)
	{
		this->parameter->desireValue = this->maxDesireValue;
		this->isPriority = true;
		this->isChangeColorScale = true;
		this->isInitializeColorScale = false;
	}

	/*�Ⴄ��ԂȂ猻�݂̏�Ԃ�ۑ�����*/
	else
	{
		this->prevState = nowState;
	}

}