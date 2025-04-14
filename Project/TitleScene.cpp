#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "TitleScene.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "SceneChanger.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene()
{
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void TitleScene::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& map			= Singleton<MapManager>::GetInstance();
	auto& camera		= Singleton<CameraManager>::GetInstance();
	auto& player		= Singleton<PlayerManager>::GetInstance();
	auto& enemy			= Singleton<EnemyManager>::GetInstance();
	auto& ui			= Singleton<UIManager>::GetInstance();
	auto& sound			= Singleton<SoundManager>::GetInstance();

	/*初期化*/
	//camera.Initialize();
	player.Initialize();
	enemy.Initialize();
	map.Initialize();
	ui.Initialize();
	sound.OnIsPlayBgm(SoundManager::BgmType::TITLE_BGM);
}

/// <summary>
/// 最終処理
/// </summary>
void TitleScene::Finalize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& map = Singleton<MapManager>::GetInstance();

	map.Finalize();
}

/// <summary>
/// 更新
/// </summary>
void TitleScene::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input			= Singleton<InputManager>::GetInstance();
	auto& camera		= Singleton<CameraManager>	::GetInstance();
	auto& map			= Singleton<MapManager>		::GetInstance();
	auto& ui			= Singleton<UIManager>		::GetInstance();
	auto& sceneChanger	= Singleton<SceneChanger>	::GetInstance();
	auto& collision = Singleton<CollisionManager>::GetInstance();
	auto& sound = Singleton<SoundManager>::GetInstance();

	/*更新処理*/
	collision.Update();
	input.Update();
	camera.Update();
	map.Update();
	ui.Update();
	sound.Update();

	/*シーンの終了処理*/
	if (this->IsEnd())
	{
		sceneChanger.ChangeScene(SceneChanger::SceneType::SELECT);
	}
}

/// <summary>
/// 描画
/// </summary>
const void TitleScene::Draw()const
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
bool TitleScene::IsEnd()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& ui = Singleton<UIManager>::GetInstance();

	/*タイトルシーンが終了可能かつ終了ボタンが押されていたらtrueを返す*/
	if (ui.IsDraw())
	{
		return true;
	}
	return false;
}