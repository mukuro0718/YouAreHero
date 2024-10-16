#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "TutorialScene.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "SceneChanger.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
TutorialScene::TutorialScene()
{
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
TutorialScene::~TutorialScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void TutorialScene::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>	 ::GetInstance();

	/*初期化*/
	player.Initialize();
	enemy.Initialize();
	//camera.Initialize();
	map.Initialize();
	ui.Initialize();

	this->isPrevPressAnyButton = false;
}

/// <summary>
/// 最終処理
/// </summary>
void TutorialScene::Finalize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& map = Singleton<MapManager>::GetInstance();

	map.Finalize();
}

/// <summary>
/// 更新
/// </summary>
void TutorialScene::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& camera		= Singleton<CameraManager>	::GetInstance();
	auto& map			= Singleton<MapManager>		::GetInstance();
	auto& ui			= Singleton<UIManager>		::GetInstance();
	auto& sceneChanger  = Singleton<SceneChanger>	::GetInstance();
	auto& input			= Singleton<InputManager>::GetInstance();

	/*更新処理*/
	input.Update();
	camera.Update();
	map.Update();
	ui.Update();

	/*シーンの終了処理*/
	if (this->IsEnd())
	{
		sceneChanger.ChangeScene(SceneChanger::SceneType::GAME);
	}
}

/// <summary>
/// 描画
/// </summary>
const void TutorialScene::Draw()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& map	 = Singleton<MapManager>	::GetInstance();
	auto& camera = Singleton<CameraManager>	::GetInstance();
	auto& ui	 = Singleton<UIManager>		::GetInstance();

	/*描画*/
	camera.Draw();
	map.Draw();
	ui.Draw();
}

/// <summary>
/// シーンを終了するか
/// </summary>
bool TutorialScene::IsEnd()
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