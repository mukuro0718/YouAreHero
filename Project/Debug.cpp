#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "Debug.h"
#include "InputManager.h"
#include "BitFlag.h"

const int Debug::COLOR_WHITE = GetColor(255, 255, 255);//白色
const int Debug::COLOR_BLACK = GetColor(0, 0, 0);//黒色

/// <summary>
/// コンストラクタ
/// </summary>
Debug::Debug()
	: flags(nullptr)
	, currentlySelectedItem(0)
	, previousSelectedItem(0)
	, isShow(false)
{
	this->flags = new BitFlag();
	;
	/*関数mapの設定*/
	auto cameraSet	 = [this]() { this->flags->SetFlag (this->CAMERA); };
	auto cameraClear = [this]() { this->flags->ClearFlag(this->CAMERA); };
	auto playerSet	 = [this]() { this->flags->SetFlag  (this->PLAYER); };
	auto playerClear = [this]() { this->flags->ClearFlag(this->PLAYER); };
	auto fpsSet		 = [this]() { this->flags->SetFlag  (this->FPS); };
	auto fpsClear	 = [this]() { this->flags->ClearFlag(this->FPS); };
	AddItemFunction(static_cast<int>(ItemType::CAMERA), cameraSet, cameraClear);
	AddItemFunction(static_cast<int>(ItemType::PLAYER), playerSet, playerClear);
	AddItemFunction(static_cast<int>(ItemType::FPS), fpsSet	  , fpsClear);
}

/// <summary>
/// デストラクタ
/// </summary>
Debug::~Debug() 
{

}

/// <summary>
/// 初期化
/// </summary>
void Debug::Initialize()
{
	/*すべてが１のマスクビットを使って、すべてのビットを下す*/
	this->flags->ClearFlag(this->MASK_ALL_FLAGS);
	this->currentlySelectedItem = 0;
	this->previousSelectedItem = this->currentlySelectedItem;
	this->isShow = false;
}

/// <summary>
/// 更新
/// </summary>
void Debug::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = InputManager::GetInstance();

	/*PAD入力の取得*/
	int pad = input.GetPadState();
	
	/*デバック項目の表示判定*/
	this->isShow = IsShow();

	/*選択項目の変更*/
	ChangeSelectedItem();

	/*on/off切り替え*/
	ChangeFlagsState();
}

/// <summary>
/// 描画
/// </summary>
const void Debug::Draw()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json = JsonManager::GetInstance();
	
	/*jsonデータを定数型に代入*/
	const int ITEM_NUM   = json.GetJson(JsonManager::FileType::DEBUG)["ITEM_NUM"];
	
	printfDx("O:open B:close Arrow\n");

	if (this->isShow)
	{
		printfDx("DEBUG_MENU\n");
		for (int i = 0; i < ITEM_NUM; i++)
		{
			switch (i)
			{
			case static_cast<int>(ItemType::CAMERA):
				printfDx("%d:CAMERA  ", this->flags->CheckFlag(this->CAMERA));
				break;
			case static_cast<int>(ItemType::PLAYER):
				printfDx("%d:PLAYER  ", this->flags->CheckFlag(this->PLAYER));
				break;
			case static_cast<int>(ItemType::FPS):
				printfDx("%d:FPS  ", this->flags->CheckFlag(this->FPS));
				break;
			}
			if (i == this->currentlySelectedItem)printfDx("<=\n");
			else printfDx("\n");
		}
	}
}


const bool Debug::IsShow()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = InputManager::GetInstance();

	/*メニューが開かれていなかったら*/
	if (!this->isShow)
	{
		//コントローラーのstartボタンまたはOキーが押されていなかったらfalseを返す
		if (!(CheckHitKey(KEY_INPUT_O))) return false;
	}
	else
	{
		//コントローラーのBボタンまたはBキーが押されていたらfalseを返す
		if (CheckHitKey(KEY_INPUT_B)) return false;
	}

	/*上記以外だったらtrueを返す*/
	return true;
}

/// <summary>
/// 選択されている項目の変更
/// </summary>
void Debug::ChangeSelectedItem()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& json  = JsonManager::GetInstance();
	auto& input = InputManager::GetInstance();

	/*定数型に代入*/
	const int ITEM_NUM	= json.GetJson(JsonManager::FileType::DEBUG)["ITEM_NUM"];//項目の数
	
	/*上下入力がない*/
	if (CheckHitKeyAll(DX_CHECKINPUT_KEY) == 0)
	{
		//以前と今の項目が一致していたらリターンを返す
		if (this->previousSelectedItem == this->currentlySelectedItem) return;
		/*項目を一致させる*/
		this->previousSelectedItem = this->currentlySelectedItem;
	}
	else
	{
		//以前と今の項目が一致していなかったらリターンを返す
		if (this->previousSelectedItem != this->currentlySelectedItem) return;

		//上入力があったら
		if (CheckHitKey(KEY_INPUT_UP))
		{
			this->currentlySelectedItem--;
			if (this->currentlySelectedItem < 0)
			{
				this->currentlySelectedItem = 0;
			}
		}
		//下入力があったら
		else if(CheckHitKey(KEY_INPUT_DOWN))
		{
			this->currentlySelectedItem++;
			if (this->currentlySelectedItem > ITEM_NUM)
			{
				this->currentlySelectedItem = ITEM_NUM;
			}
		}
	}
}

void Debug::ChangeFlagsState()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = InputManager::GetInstance();

	/*入力がなければリターンを返す*/
	if (CheckHitKeyAll(DX_CHECKINPUT_KEY) == 0)return;

	/*右入力*/
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		this->itemFunctionMap[this->currentlySelectedItem].set();
	}
	/*左入力*/
	else if (CheckHitKey(KEY_INPUT_LEFT))
	{
		this->itemFunctionMap[this->currentlySelectedItem].clear();
	}

}

/// <summary>
/// 項目ごとの関数の追加
/// </summary>
void Debug::AddItemFunction(const int _item, const FlagsState _set, const FlagsState _clear)
{
	FlagsStateSet add;
	add.set = _set;
	add.clear = _clear;
	this->itemFunctionMap.emplace(_item, add);
}

/// <summary>
/// マップ内のビットフラグを取得
/// </summary>
/// <returns></returns>
const unsigned int Debug::GetBitFlagInMap(const int _itemName)const
{
	switch (_itemName)
	{
	case static_cast<int>(ItemType::CAMERA):
		return this->CAMERA;
	case static_cast<int>(ItemType::PLAYER):
		return this->PLAYER;
	case static_cast<int>(ItemType::FPS):
		return this->FPS;
	}
	return 0;
}
/// <summary>
/// カメラのデバック機能がONになっているかどうか
/// </summary>
const bool Debug::CheckCameraFlag()const 
{ 
	return this->flags->CheckFlag(this->CAMERA);
}
/// <summary>
/// FPSのデバック機能がONになっているかどうか
/// </summary>
const bool Debug::CheckFPSFlag()const
{
	return this->flags->CheckFlag(this->FPS);
}