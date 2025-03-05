#include <DxLib.h>
#include "UseSTL.h"
#include "TutorialManager.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TutorialManager::TutorialManager()
	: stage			()
	, isMoveCamera	(false)
	, isLockon		(false)
	, isWalk		(false)
	, isRun			(false)
	, isWeakAttack	(false)
	, isRotateAttack(false)
	, isAvoid		(false)
	, isBlock		(false)
	, isHeal		(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TutorialManager::~TutorialManager()
{
}

/// <summary>
/// ������
/// </summary>
void TutorialManager::Initialize()
{
	this->stage = 0;
	this->isMoveCamera	 = false;
	this->isLockon		 = false;
	this->isWalk		 = false;
	this->isRun			 = false;
	this->isWeakAttack	 = false;
	this->isRotateAttack = false;
	this->isAvoid		 = false;
	this->isBlock		 = false;
	this->isHeal		 = false;
}

/// <summary>
/// �X�V
/// </summary>
void TutorialManager::Update()
{
	auto& input = Singleton<InputManager>::GetInstance();
	InputManager::InputAnalogStick rStick = input.GetRStickState();
	InputManager::InputAnalogStick lStick = input.GetLStickState();
	switch (this->stage)
	{
	case static_cast<int>(Stage::CAMERA):
		//�J��������
		if (!this->isMoveCamera && (rStick.XBuf != 0 || rStick.YBuf != 0))
		{
			this->isMoveCamera = true;
		}
		//�X�e�B�b�N��������
		if (!this->isLockon && input.GetNowPad(InputManager::PAD_RS))
		{
			this->isLockon = true;
		}
		if (this->isMoveCamera && this->isLockon)
		{
			this->stage++;
		}
		break;
	case static_cast<int>(Stage::MOVE):
		//����
		if (!this->isWalk && (lStick.XBuf != 0 || lStick.YBuf != 0))
		{
			this->isWalk = true;
		}
		//�_�b�V��
		if (!this->isRun && (lStick.XBuf != 0 || lStick.YBuf != 0) && input.GetNowPad(InputManager::PAD_RB))
		{
			this->isRun = true;
		}
		if (this->isWalk && this->isRun)
		{
			this->stage++;
		}
		break;
	case static_cast<int>(Stage::ATTACK):
		//��U��
		if (!this->isWeakAttack && input.GetNowPad(InputManager::PAD_B))
		{
			this->isWeakAttack = true;
		}
		//��]�U��
		if (!this->isRotateAttack && input.GetNowPad(InputManager::PAD_Y))
		{
			this->isRotateAttack = true;
		}
		if (this->isRotateAttack && this->isWeakAttack)
		{
			this->stage++;
		}
		break;
	case static_cast<int>(Stage::AVOID):
		//���
		if (!this->isAvoid && input.GetNowPad(InputManager::PAD_A))
		{
			this->isAvoid = true;
		}
		//�K�[�h
		if (!this->isBlock && input.GetLeftTrigger())
		{
			this->isBlock = true;
		}
		if (this->isAvoid && this->isBlock)
		{
			this->stage++;
		}
		break;
	case static_cast<int>(Stage::HEAL):
		//��
		if (!this->isHeal && input.GetNowPad(InputManager::PAD_X))
		{
			this->isHeal = true;
		}
		if (this->isHeal)
		{
			this->stage++;
		}
		break;
	default:
		break;
	}
}