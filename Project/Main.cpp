#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "ScreenSetup.h"
#include "SceneManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    /*スクリーン設定*/
    auto& screenSetup = Singleton<ScreenSetup>::GetInstance();
    if (screenSetup.Initialize() == -1) return -1; 

    /*ゲーム本編*/
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        auto& scene = Singleton<SceneManager>::GetInstance();
        scene.Update();
    }

    // Effekseerを終了する。
    Effkseer_End();
    // ＤＸライブラリの後始末
    DxLib_End();

    // ソフトの終了
    return 0;
}