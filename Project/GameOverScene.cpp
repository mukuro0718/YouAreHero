#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "GameOverScene.h"
#include "Character.h"
#include "Player.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "SceneChanger.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameOverScene::GameOverScene()
{
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
GameOverScene::~GameOverScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameOverScene::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*初期化*/
	//camera.Initialize();
	map.Initialize();
	ui.Initialize();

	this->isPrevPressAnyButton = false;
	if (player.GetHP() < 0)
	{
		this->isGameOver = true;
		this->isGameClear = false;
	}
	else
	{
		this->isGameOver = false;
		this->isGameClear = true;
	}
}

/// <summary>
/// 最終処理
/// </summary>
void GameOverScene::Finalize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& map = Singleton<MapManager>::GetInstance();

	map.Finalize();
}

/// <summary>
/// 更新
/// </summary>
void GameOverScene::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	/*auto& camera = Singleton<CameraManager>	::GetInstance();
	auto& map = Singleton<MapManager>		::GetInstance();
	auto& ui = Singleton<UIManager>		::GetInstance();*/
	auto& sceneChanger = Singleton<SceneChanger>	::GetInstance();
	//auto& input = Singleton<InputManager>::GetInstance();

	///*更新処理*/
	//input.Update();
	//camera.Update();
	//map.Update();
	//ui.Update();

	///*シーンの終了処理*/
	//if (this->IsEnd())
	//{
		sceneChanger.ChangeScene(SceneChanger::SceneType::SELECT);
	//}
}

/// <summary>
/// 描画
/// </summary>
const void GameOverScene::Draw()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& map = Singleton<MapManager>		 ::GetInstance();
	auto& camera = Singleton<CameraManager>	 ::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();

	/*描画*/
	camera.Draw();
	map.Draw();
	ui.Draw();
}

/// <summary>
/// シーンを終了するか
/// </summary>
bool GameOverScene::IsEnd()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();

	/*何かボタンが押されたか*/
	int pad = input.GetNowPadState();
	bool isPressAnyButton = (pad & PAD_INPUT_4);

	/*タイトルシーンが終了可能かつ終了ボタンが押されていたらtrueを返す*/
	if (isPressAnyButton && ui.IsDraw())
	{
		return true;
	}
	return false;
}