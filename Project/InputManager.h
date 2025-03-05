//==============================================================
// @brief　インプットマネージャークラス
// プロジェクト内で使用するパッド入力を保持する
// シングルトンで作成し、各クラスで入力状態を取得できるようにする
//==============================================================
#pragma once
#include "Singleton.h"

class InputManager : public Singleton<InputManager>
{
public:
	friend class Singleton<InputManager>;
	
	virtual ~InputManager();//デストラクタ
	
	/*構造体*/
	//アナログスティック
	struct InputAnalogStick
	{
		int XBuf = 0;
		int YBuf = 0;
	};
	//マウス
	struct InputMouse
	{
		int XBuf = 0;
		int YBuf = 0;
	};
	
	/*静的定数*/
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
	//キーボード
	static constexpr unsigned int KEY_ENTER = KEY_INPUT_RETURN;
	static constexpr unsigned int KEY_E		= KEY_INPUT_E;
	static constexpr unsigned int KEY_F		= KEY_INPUT_F;
	static constexpr unsigned int KEY_W		= KEY_INPUT_W;
	static constexpr unsigned int KEY_A		= KEY_INPUT_A;
	static constexpr unsigned int KEY_S		= KEY_INPUT_S;
	static constexpr unsigned int KEY_D		= KEY_INPUT_D;
	//マウス
	static constexpr unsigned int MOUSE_LEFT = MOUSE_INPUT_1;
	static constexpr unsigned int MOUSE_RIGHT = MOUSE_INPUT_2;
	void Update();//更新

	/*getter*/
	const bool GetNowPadIntoPrevPad (const int _buttonType)const;												//pad状態のgetter
	const bool GetNowPad			(const int _buttonType)const { return this->nowPad.Buttons[_buttonType]; }	//pad状態のgetter
	const bool GetPrevPad			(const int _buttonType)const { return this->prevPad.Buttons[_buttonType]; }	//pad状態のgetter
	const bool GetLeftTrigger		() { return this->nowPad.LeftTrigger; }
	const bool GetRightTrigger		() { return this->nowPad.RightTrigger; }
	//const bool GetReturnKeyState()const { return this->isInputReturnKey; }//key状態のgetter
	//const bool GetComKeysForMoveState(const ComKeysForMove _index)const { return this->moveKey[convMoveKey.at(_index)]; }//移動キーの状態のgetter
	//const bool GetArrowKeyState(const ArrowKey _index)const { return this->arrowKey[convArrowKey.at(_index)]; }//矢印キーの状態のgetter
	const InputAnalogStick GetLStickState()const { return this->lStick; }//左スティック状態のgetter
	const InputAnalogStick GetRStickState()const { return this->rStick; }//右スティック状態のgetter
	//const DINPUT_JOYSTATE  GetButtonState()const { return this->button; }//十字ボタン入力取得用ボタン変数
private:
	/*静的定数*/
	static constexpr int ARROW_KEY_NUM = 4;//矢印キーの数
	static constexpr int COM_KEYS_FOR_MOVE = 6;//移動キーの数

	/*内部処理関数*/
	InputManager();//コンストラクタ
	void InputPadState();//PAD入力の取得
	void InputKeyState();//キーボード入力の取得
	void InputMouseState();//キーボード入力の取得

	/*メンバ変数*/
	InputAnalogStick lStick;	//lStick
	InputAnalogStick rStick;	//rStick
	InputMouse		 mouse;		//マウス
	XINPUT_STATE	 nowPad;
	XINPUT_STATE	 prevPad;
	//int			 nowPad;	//パッド入力変数
	//int			 prevPad;	//パッド入力変数
	int				 nowKey;	//キー入力変数
	int				 prevKey;	//キー入力変数
	int				 nowMouse;	//マウスボタン入力変数
	int				 prevMouse;	//マウスボタン入力変数
};

