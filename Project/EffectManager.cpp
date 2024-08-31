#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include <vector>
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EffectManager::EffectManager()
{
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

}

/// <summary>
/// 初期化
/// </summary>
void EffectManager::Initialize()
{

}

/// <summary>
/// 後処理
/// </summary>
void EffectManager::Finalize()
{

}