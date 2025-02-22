#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"	
#include "DeleteInstance.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "ReactionType.h"
#include "ActionParameter.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "BossAttack.h"
#include "Boss.h"
#include "BossAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAction::BossAction()
	: parameter				(nullptr)
	, attack				(nullptr)
	, moveTarget			(Gori::ORIGIN)
	, isSelect				(false)
	, frameCount			(0)
	, isInitialize			(false)
	, isPriority			(false)
	, isAllowAction			(false)
	, frameTime				(0)
	, nextAnimation			(0)
	, animationPlayTime		(0.0f)
	, maxDesireValue		(0)
	, nowTotalAnimPlayTime	(0.0f)
	, interval				(0)
{
	/*�C���X�^���X�̍쐬*/
	this->parameter = new ActionParameter();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossAction::~BossAction()
{
	DeleteMemberInstance(this->attack);
	DeleteMemberInstance(this->parameter);
}
/// <summary>
/// �`��
/// </summary>
const void BossAction::Draw()const
{
	//if (this->attack != nullptr)
	//{
	//	this->attack->Draw();
	//}
	//printfDx("ENEMY_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// �~���l�̎擾
/// </summary>
const int BossAction::GetDesireValue() const
{
	return static_cast<int>(this->parameter->desireValue);
}

/// <summary>
/// �d�݂̎擾
/// </summary>
const int BossAction::GetWeight(const int _sum)const
{
	return this->parameter->GetWeight(_sum);
}

/// <summary>
/// �d�݂̃x�[�X�̎擾
/// </summary>
const int BossAction::GetBaseWeight()const
{
	return static_cast<int>(this->parameter->BASE_WEIGHT);
}
/// <summary>
/// �w�肵���t���[���̌v��
/// </summary>
bool BossAction::FrameCount(const int _maxFrame)
{
	//�J�E���g�𑝉�������
	this->frameCount++;
	//�����ő���z���Ă�����t���O�������ăJ�E���g������������
	if (this->frameCount >= _maxFrame)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �ϐ��̏�����
/// </summary>
void BossAction::OffIsSelect(const int _maxInterval)
{
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		this->isSelect = false;
		this->isPriority = false;
		this->isAllowAction = false;
		this->frameCount = 0;
		this->parameter->desireValue = 0;
		this->interval = _maxInterval;
	}
}

/// <summary>
/// �⊮������]���̎擾
/// </summary>
VECTOR BossAction::GetLerpRotation(Boss& _boss, const VECTOR _positionToTargetVector, const VECTOR _nowRotation, const VECTOR _lerpValue)
{
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR rotation = Gori::ORIGIN;
	//���̉�]�������߂�
	nextRotation.y = static_cast<float>(atan2(static_cast<double>(_positionToTargetVector.x), static_cast<double>(_positionToTargetVector.z)));
	//��]����ۊǂ���
	rotation = _boss.Lerp360Angle(_nowRotation, nextRotation, _lerpValue);
	return rotation;
}

/// <summary>
/// �ړ��x�N�g���̌v�Z
/// </summary>
VECTOR BossAction::CalcVelocity(const VECTOR _prevVelocity, const VECTOR _nowRotation, const float _speed)
{
	//��]�������ƂɈړ��x�N�g�����o��
	VECTOR direction = VGet(-sinf(_nowRotation.y), 0.0f, -cosf(_nowRotation.y));
	//�ړ��x�N�g���𐳋K��
	direction = VNorm(direction);
	//�V�����ړ��x�N�g�����o���i�d�͂��l�����āAY�����̂ݑO�̂��̂��g�p����j
	VECTOR aimVelocity = VScale(direction, _speed);					 //�Z�o���ꂽ�ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, _prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��

	return newVelocity;
}


/// <summary>
/// ���[�v
/// </summary>
float BossAction::Lerp(const float _start, const float _end, const float _percent)
{
	return _start + _percent * (_end - _start);
}
COLOR_F BossAction::LerpColor(const COLOR_F _start, const COLOR_F _end, const COLOR_F _percent)
{
	COLOR_F out;
	out.r = Lerp(_start.r, _end.r, _percent.r);
	out.g = Lerp(_start.g, _end.g, _percent.g);
	out.b = Lerp(_start.b, _end.b, _percent.b);
	out.a = Lerp(_start.a, _end.a, _percent.a);
	return out;
}

/// <summary>
/// vector<float>��COLOR_F�ɕϊ�
/// </summary>
const COLOR_F BossAction::ColorConvert(const std::vector<float> _value)
{
	const COLOR_F out = { _value[0],_value[1], _value[2],_value[3] };
	return out;
}
