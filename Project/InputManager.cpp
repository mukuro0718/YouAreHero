#include <DxLib.h>
#include "InputManager.h"

//InputManager* Singleton<InputManager>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
InputManager::InputManager()
	: lStick{ 0,0 }
	, rStick{0,0}
{
	
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
InputManager::~InputManager()
{

}
/// <summary>
/// PAD���͂̎擾
/// </summary>
void InputManager::InputPadState()
{
	/*�{�^�����͂̎�t*/
	//this->nowPad = GetJoypadInputState(DX_INPUT_PAD1);
	this->prevPad = this->nowPad;
	GetJoypadXInputState(DX_INPUT_PAD1,&this->nowPad);

	/*���X�e�B�b�N���͂̎擾*/
	GetJoypadAnalogInput(&this->lStick.XBuf, &this->lStick.YBuf, DX_INPUT_PAD1);

	/*�E�X�e�B�b�N�̓���*/
	GetJoypadAnalogInputRight(&this->rStick.XBuf, &this->rStick.YBuf, DX_INPUT_PAD1);
}
void InputManager::Update()
{
	InputPadState();
	InputKeyState();
}
/// <summary>
/// �L�[�{�[�h���͂̎擾
/// </summary>
void InputManager::InputKeyState()
{
}
/// <summary>
/// �}�E�X���͂̎擾
/// </summary>
void InputManager::InputMouseState()
{
}

/// <summary>
/// 1�t���[���O�̓��͏󋵂������������ʂ�Ԃ�
/// </summary>
const bool InputManager::GetNowPadIntoPrevPad(const int _buttonType)const
{
	if (this->nowPad.Buttons[_buttonType] && !this->prevPad.Buttons[_buttonType])
	{
		return true;
	}
	return false;
}
