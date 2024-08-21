#include <DxLib.h>
#include "UseJson.h"
#include "ScreenSetup.h"

/// <summary>
/// コンストラクタ
/// </summary>
ScreenSetup::ScreenSetup()
{
    Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
ScreenSetup::~ScreenSetup()
{

}

int ScreenSetup::Initialize()
{

    /*Jsonマネージャーのインスタンスの取得*/
    auto& json = Singleton<JsonManager>::GetInstance();

    /*仮の変数に代入*/
    int windowHeight = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["WINDOW_HEIGHT"];
    int windowWidth  = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["WINDOW_WIDTH"];
    int colorBit     = json.GetJson(JsonManager::FileType::SCREEN_SETUP)["COLOR_BIT"];

    SetBackgroundColor(100, 100, 100);

    // ＤＸライブラリの初期化
    if (DxLib_Init() == -1)
    {
        // エラーが発生したら直ちに終了
        return -1;
    }
	/*画面の作成*/
    SetGraphMode(windowWidth, windowHeight, colorBit);
    //フラグを立てることで非同期ロードが可能になる
    SetUseASyncLoadFlag(TRUE);
    //ScreenFlipを実行しても垂直同期信号を待たない
    SetWaitVSyncFlag(FALSE);
    //描画先を裏画面にする
    SetDrawScreen(DX_SCREEN_BACK);
    //画面モードをウィンドウモードで描画するか
#if _DEBUG
    ChangeWindowMode(TRUE);
#else
    ChangeWindowMode(FALSE);
#endif// _DEBUG
    return 0;
}