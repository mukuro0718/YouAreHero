#include <DxLib.h>
#include "InputManager.h"

//InputManager* Singleton<InputManager>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
InputManager::InputManager()
	: pad(0)
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
	this->pad = GetJoypadInputState(DX_INPUT_PAD1);
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
	///*変数の初期化*/
	//for (int i = 0; i < moveKey.size(); i++)
	//{
	//	moveKey[i] = false;
	//}
	//for (int i = 0; i < arrowKey.size(); i++)
	//{
	//	arrowKey[i] = false;
	//}

	///*エンター入力の取得*/
	//if (!this->isPrevInputReturnKey && CheckHitKey(KEY_INPUT_RETURN))
	//{
	//	this->isInputReturnKey = true;
	//	this->isPrevInputReturnKey = true;
	//}
	//else
	//{
	//	this->isInputReturnKey = false;
	//	if (!CheckHitKey(KEY_INPUT_RETURN))
	//	{
	//		this->isPrevInputReturnKey = false;
	//	}
	//}

	///*移動キー入力の取得*/
	//if (CheckHitKey(KEY_INPUT_W))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::W]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_D))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::D]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_S))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::S]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_A))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::A]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_SPACE))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::SPACE]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_LCONTROL))
	//{
	//	this->moveKey[convMoveKey[ComKeysForMove::LCONTROL]] = true;
	//}

	///*矢印キー入力の取得*/
	//if (CheckHitKey(KEY_INPUT_RIGHT))
	//{
	//	this->arrowKey[convArrowKey[ArrowKey::RIGHT]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_LEFT))
	//{
	//	this->arrowKey[convArrowKey[ArrowKey::LEFT]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_UP))
	//{
	//	this->arrowKey[convArrowKey[ArrowKey::UP]] = true;
	//}
	//if (CheckHitKey(KEY_INPUT_DOWN))
	//{
	//	this->arrowKey[convArrowKey[ArrowKey::DOWN]] = true;
	//}
}
