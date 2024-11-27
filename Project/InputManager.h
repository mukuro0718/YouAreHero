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
	
	/*アナログスティック構造体*/
	struct InputAnalogStick
	{
		int XBuf = 0;
		int YBuf = 0;
	};
	
	/*移動に使用する一般的なキー*/
	enum class ComKeysForMove
	{
		W,
		A,
		S,
		D,
		SPACE,	 //上昇
		LCONTROL,//下降
	};

	/*矢印キー*/
	enum class ArrowKey
	{
		RIGHT,
		LEFT,
		UP,
		DOWN
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
	static constexpr unsigned int PAD_RT = PAD_INPUT_8;

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

	/*メンバ変数*/
	InputAnalogStick lStick;
	InputAnalogStick rStick;
	int nowPad;//パッド入力変数
	int prevPad;//パッド入力変数
};

