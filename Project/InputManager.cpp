#include <DxLib.h>
#include "InputManager.h"

//InputManager* Singleton<InputManager>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
InputManager::InputManager()
	: nowPad(0)
	, prevPad(0)
	, lStick{0,0}
	, rStick{0,0}
{
	
}
/// <summary>
/// デストラクタ
/// </summary>
InputManager::~InputManager()
{

}
/// <summary>
/// PAD入力の取得
/// </summary>
void InputManager::InputPadState()
{
	/*ボタン入力の受付*/
	this->prevPad = this->nowPad;
	this->nowPad = GetJoypadInputState(DX_INPUT_PAD1);
	/*左スティック入力の取得*/
	GetJoypadAnalogInput(&this->lStick.XBuf, &this->lStick.YBuf, DX_INPUT_PAD1);
	/*右スティックの入力*/
	GetJoypadAnalogInputRight(&this->rStick.XBuf, &this->rStick.YBuf, DX_INPUT_PAD1);
}
void InputManager::Update()
{
	InputPadState();
	InputKeyState();
}
/// <summary>
/// キーボード入力の取得
/// </summary>
void InputManager::InputKeyState()
{
}
/// <summary>
/// マウス入力の取得
/// </summary>
void InputManager::InputMouseState()
{
}