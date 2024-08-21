#include <DxLib.h>
#include "UseSTL.h"
#include "Animation.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Animation::Animation()
	: animationAttachIndex(0)
	, prevAnimation(0)
	, nowAnimation(0)
	, animationRate(0.0f)
	, prevAnimationAttachIndex(0)
	, animationTotalTime(0.0f)
	, animationPlayTime(0.0f)
	, isChange(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Animation::~Animation()
{
	this->animationHandle.clear();
	this->animationIndex.clear();
}

/// <summary>
/// �A�j���[�V�����̒ǉ�
/// </summary>
void Animation::Add(const int _animationHandle,const int _animatinoIndex)
{
	this->animationHandle.push_back(_animationHandle);
	this->animationIndex.push_back(_animatinoIndex);
}

/// <summary>
/// �A�j���[�V�����̃A�^�b�`
/// </summary>
void Animation::Attach(int* modelHandle)
{
	//�A�j���[�V�����̃f�^�b�`
	MV1DetachAnim(*modelHandle, this->prevAnimationAttachIndex);
	MV1DetachAnim(*modelHandle, this->animationAttachIndex);

	//�A�j���[�V�����̃A�^�b�`
	this->prevAnimationAttachIndex = MV1AttachAnim(*modelHandle, this->animationIndex[this->prevAnimation], this->animationHandle[this->prevAnimation], FALSE);
	this->animationAttachIndex = MV1AttachAnim(*modelHandle, this->animationIndex[this->nowAnimation], this->animationHandle[this->nowAnimation], FALSE);

	//�A�j���[�V�����̑��Đ����Ԃ�ݒ�
	this->animationTotalTime = MV1GetAttachAnimTotalTime(*modelHandle, this->animationAttachIndex);
	this->animationRate = 0.0f;
}
/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void Animation::Play(int* modelHandle, const int _nextAnimation, const float _animationPlayTime)
{
	/*�������܂ŃA�^�b�`���Ă����A�j���[�V�����Ǝ��̃A�j���[�V�������Ⴄ�Ȃ�*/
	if (this->nowAnimation != _nextAnimation)
	{
		this->prevAnimation = this->nowAnimation;
		this->nowAnimation = _nextAnimation;
		this->isChange = false;
		this->animationPlayTime = 0.0f;
		//�A�j���[�V�����̃A�^�b�`
		Attach(modelHandle);
	}

	/*�A�j���[�V�����̃u�����h�����Z�b�g*/
	MV1SetAttachAnimBlendRate(*modelHandle, this->prevAnimationAttachIndex, 1.0f - this->animationRate);
	MV1SetAttachAnimBlendRate(*modelHandle, this->animationAttachIndex, this->animationRate);

	/*�u�����h����1�ȏゾ������*/
	if (this->animationRate >= 1.0f)
	{
		//�O�̃A�^�b�`���Ă����A�j���[�V�������f�^�b�`����
		MV1DetachAnim(*modelHandle, this->prevAnimationAttachIndex);
	}
	else
	{
		//�u�����h���𑝉�
		this->animationRate += 0.1f;
	}

	/*�A�j���[�V�����Đ����Ԃ�ݒ�*/
	MV1SetAttachAnimTime(*modelHandle, this->animationAttachIndex, this->animationPlayTime);

	/*�A�j���[�V�����Đ����Ԃ�i�߂�*/
	this->animationPlayTime += _animationPlayTime;

	/*�Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�*/
	if (this->animationPlayTime >= this->animationTotalTime)
	{
		this->animationPlayTime = 0.0f;
		this->isChange = true;
	}
}