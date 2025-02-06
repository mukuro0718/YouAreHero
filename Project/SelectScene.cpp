#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SelectScene.h"
#include "MapManager.h"
#include "UIManager.h"
#include "SceneChanger.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
SelectScene::SelectScene()
{
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
SelectScene::~SelectScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void SelectScene::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& sound = Singleton<SoundManager>::GetInstance();

	/*初期化*/
	ui.Initialize();
}

/// <summary>
/// 最終処理
/// </summary>
void SelectScene::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void SelectScene::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& ui			= Singleton<UIManager>		::GetInstance();
	auto& sceneChanger  = Singleton<SceneChanger>	::GetInstance();
	auto& input			= Singleton<InputManager>::GetInstance();

	/*更新処理*/
	input.Update();
	ui.Update();

	/*フレームカウントの計測*/

	/*シーンの終了処理*/
	if (this->IsEnd())
	{
		if (ui.GetIsBackTitle())
		{
			sceneChanger.ChangeScene(SceneChanger::SceneType::TITLE);
		}
		else
		{
			auto& sound = Singleton<SoundManager>::GetInstance();
			sound.OffIsPlayBgm(SoundManager::BgmType::TITLE_BGM);
			sceneChanger.ChangeScene(SceneChanger::SceneType::GAME);
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void SelectScene::Draw()const
{
	/*UIの描画*/
	auto& ui = Singleton<UIManager>::GetInstance();
	ui.Draw();
}

/// <summary>
/// シーンを終了するか
/// </summary>
bool SelectScene::IsEnd()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& ui = Singleton<UIManager>::GetInstance();

	/*タイトルシーンが終了可能だったらtrueを返す*/
	if (ui.IsDraw())
	{
		return true;
	}
	return false;
}