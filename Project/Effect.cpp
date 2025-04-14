#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "DeleteInstance.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Effect.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Effect::Effect(const int _effectResourceHandle)
	: effectResourceHandle	(_effectResourceHandle)
	, isPlayEffect			(false)
	, transform				(nullptr)
	, frameCount			(0)
	, startFrame			(0)
	, endFrame				(0)
{
	this->transform = new Transform();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Effect::~Effect()
{
	DeleteMemberInstance(this->transform);
}

/// <summary>
/// ������
/// </summary>
void Effect::Initialize()
{
	this->frameCount		  = 0;
	this->isPlayEffect		  = false;
}

/// <summary>
/// �`��
/// </summary>
const void Effect::Draw()const
{
	/*�Đ��t���O�������Ă��Ȃ��܂��͊J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��*/
	if (!this->isPlayEffect || this->frameCount < this->startFrame)return;
	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();
}

/// <summary>
/// ���W�̃Z�b�g
/// </summary>
void Effect::SetPosition(const VECTOR _position)
{
	this->transform->SetPosition(_position);
}

/// <summary>
/// ��]���̃Z�b�g
/// </summary>
void Effect::SetRotation(const VECTOR _rotation)
{
	this->transform->SetRotation(_rotation);
}