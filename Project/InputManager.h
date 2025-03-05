//==============================================================
// @brief�@�C���v�b�g�}�l�[�W���[�N���X
// �v���W�F�N�g���Ŏg�p����p�b�h���͂�ێ�����
// �V���O���g���ō쐬���A�e�N���X�œ��͏�Ԃ��擾�ł���悤�ɂ���
//==============================================================
#pragma once
#include "Singleton.h"

class InputManager : public Singleton<InputManager>
{
public:
	friend class Singleton<InputManager>;
	
	virtual ~InputManager();//�f�X�g���N�^
	
	/*�\����*/
	//�A�i���O�X�e�B�b�N
	struct InputAnalogStick
	{
		int XBuf = 0;
		int YBuf = 0;
	};
	//�}�E�X
	struct InputMouse
	{
		int XBuf = 0;
		int YBuf = 0;
	};
	
	/*�ÓI�萔*/
	//X_input
	static constexpr int PAD_A		= XINPUT_BUTTON_A;
	static constexpr int PAD_B		= XINPUT_BUTTON_B;
	static constexpr int PAD_X		= XINPUT_BUTTON_X;
	static constexpr int PAD_Y		= XINPUT_BUTTON_Y;
	static constexpr int PAD_LB	= XINPUT_BUTTON_LEFT_SHOULDER;
	static constexpr int PAD_RB	= XINPUT_BUTTON_RIGHT_SHOULDER;
	static constexpr int PAD_BACK	= XINPUT_BUTTON_BACK;
	static constexpr int PAD_START = XINPUT_BUTTON_START;
	static constexpr int PAD_LS	= XINPUT_BUTTON_LEFT_THUMB;
	static constexpr int PAD_RS	= XINPUT_BUTTON_RIGHT_THUMB;
	//�L�[�{�[�h
	static constexpr unsigned int KEY_ENTER = KEY_INPUT_RETURN;
	static constexpr unsigned int KEY_E		= KEY_INPUT_E;
	static constexpr unsigned int KEY_F		= KEY_INPUT_F;
	static constexpr unsigned int KEY_W		= KEY_INPUT_W;
	static constexpr unsigned int KEY_A		= KEY_INPUT_A;
	static constexpr unsigned int KEY_S		= KEY_INPUT_S;
	static constexpr unsigned int KEY_D		= KEY_INPUT_D;
	//�}�E�X
	static constexpr unsigned int MOUSE_LEFT = MOUSE_INPUT_1;
	static constexpr unsigned int MOUSE_RIGHT = MOUSE_INPUT_2;
	void Update();//�X�V

	/*getter*/
	const bool GetNowPadIntoPrevPad (const int _buttonType)const;												//pad��Ԃ�getter
	const bool GetNowPad			(const int _buttonType)const { return this->nowPad.Buttons[_buttonType]; }	//pad��Ԃ�getter
	const bool GetPrevPad			(const int _buttonType)const { return this->prevPad.Buttons[_buttonType]; }	//pad��Ԃ�getter
	const bool GetLeftTrigger		() { return this->nowPad.LeftTrigger; }
	const bool GetRightTrigger		() { return this->nowPad.RightTrigger; }
	//const bool GetReturnKeyState()const { return this->isInputReturnKey; }//key��Ԃ�getter
	//const bool GetComKeysForMoveState(const ComKeysForMove _index)const { return this->moveKey[convMoveKey.at(_index)]; }//�ړ��L�[�̏�Ԃ�getter
	//const bool GetArrowKeyState(const ArrowKey _index)const { return this->arrowKey[convArrowKey.at(_index)]; }//���L�[�̏�Ԃ�getter
	const InputAnalogStick GetLStickState()const { return this->lStick; }//���X�e�B�b�N��Ԃ�getter
	const InputAnalogStick GetRStickState()const { return this->rStick; }//�E�X�e�B�b�N��Ԃ�getter
	//const DINPUT_JOYSTATE  GetButtonState()const { return this->button; }//�\���{�^�����͎擾�p�{�^���ϐ�
private:
	/*�ÓI�萔*/
	static constexpr int ARROW_KEY_NUM = 4;//���L�[�̐�
	static constexpr int COM_KEYS_FOR_MOVE = 6;//�ړ��L�[�̐�

	/*���������֐�*/
	InputManager();//�R���X�g���N�^
	void InputPadState();//PAD���͂̎擾
	void InputKeyState();//�L�[�{�[�h���͂̎擾
	void InputMouseState();//�L�[�{�[�h���͂̎擾

	/*�����o�ϐ�*/
	InputAnalogStick lStick;	//lStick
	InputAnalogStick rStick;	//rStick
	InputMouse		 mouse;		//�}�E�X
	XINPUT_STATE	 nowPad;
	XINPUT_STATE	 prevPad;
	//int			 nowPad;	//�p�b�h���͕ϐ�
	//int			 prevPad;	//�p�b�h���͕ϐ�
	int				 nowKey;	//�L�[���͕ϐ�
	int				 prevKey;	//�L�[���͕ϐ�
	int				 nowMouse;	//�}�E�X�{�^�����͕ϐ�
	int				 prevMouse;	//�}�E�X�{�^�����͕ϐ�
};

