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
	//D_input
	static constexpr unsigned int PAD_X = PAD_INPUT_1;
	static constexpr unsigned int PAD_Y = PAD_INPUT_2;
	static constexpr unsigned int PAD_A = PAD_INPUT_3;
	static constexpr unsigned int PAD_B = PAD_INPUT_4;
	static constexpr unsigned int PAD_LB = PAD_INPUT_5;
	static constexpr unsigned int PAD_RB = PAD_INPUT_6;
	static constexpr unsigned int PAD_LT = PAD_INPUT_7;
	static constexpr unsigned int PAD_LS = PAD_INPUT_10;
	static constexpr unsigned int PAD_RT = PAD_INPUT_8;
	static constexpr unsigned int PAD_START = PAD_INPUT_12;
	//キーボード
	static constexpr unsigned int KEY_ENTER = KEY_INPUT_RETURN;
	static constexpr unsigned int KEY_E = KEY_INPUT_E;
	static constexpr unsigned int KEY_F = KEY_INPUT_F;
	static constexpr unsigned int KEY_W = KEY_INPUT_W;
	static constexpr unsigned int KEY_A = KEY_INPUT_A;
	static constexpr unsigned int KEY_S = KEY_INPUT_S;
	static constexpr unsigned int KEY_D = KEY_INPUT_D;
	//マウス
	static constexpr unsigned int MOUSE_LEFT = MOUSE_INPUT_1;
	static constexpr unsigned int MOUSE_RIGHT = MOUSE_INPUT_2;
	void Update();//更新

	/*getter*/
	const int GetNowPadState()const { return this->nowPad; }//pad状態のgetter
	const int GetPrevPadState()const { return this->prevPad; }//pad状態のgetter
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
	InputAnalogStick lStick;
	InputAnalogStick rStick;
	InputMouse mouse;
	int nowPad;//パッド入力変数
	int prevPad;//パッド入力変数
	int nowKey;//キー入力変数
	int prevKey;//キー入力変数
	int nowMouse;//マウスボタン入力変数
	int prevMouse;//マウスボタン入力変数
};

