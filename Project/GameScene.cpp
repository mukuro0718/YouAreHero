#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "SceneBase.h"
#include "GameScene.h"
#include "BitFlag.h"
#include "SceneChanger.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "Debug.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "PlayerAttackManager.h"
#include "EnemyManager.h"
#include "UIManager.h"
#include "EffectManager.h"
#include "CollisionManager.h"
#include "Debug.h"
#include "HitStopManager.h"
#include "Timer.h"
#include "FPSController.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene()
{
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& effect = Singleton<EffectManager>		::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& debug = Singleton<Debug>::GetInstance();
	auto& timer = Singleton<Timer>::GetInstance();
	
	/*初期化*/
	//camera.Initialize();
	timer.Initialize();
	map.Initialize();
	player.Initialize();
	enemy.Initialize();
	playerAttack.Initialize();
	ui.Initialize();
	debug.Initialize();
	effect.Initialize();
}

/// <summary>
/// 最終処理
/// </summary>
void GameScene::Finalize()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& map = Singleton<MapManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();

	map.Finalize();
	enemy.Finalize();
	player.Finalize();
	playerAttack.Finalize();

}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input = Singleton<InputManager>		::GetInstance();
	auto& debug = Singleton<Debug>				::GetInstance();
	auto& camera = Singleton<CameraManager>		::GetInstance();
	auto& map = Singleton<MapManager>			::GetInstance();
	auto& player = Singleton<PlayerManager>		::GetInstance();
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>		::GetInstance();
	auto& effect = Singleton<EffectManager>		::GetInstance();
	auto& ui = Singleton<UIManager>			::GetInstance();
	auto& collision = Singleton<CollisionManager>	::GetInstance();
	auto& sceneChanger = Singleton<SceneChanger>		::GetInstance();
	auto& hitStop = Singleton<HitStopManager>		::GetInstance();
	auto& timer = Singleton<Timer>::GetInstance();

	/*更新処理*/
	timer.Update();
	debug.Update();
	input.Update();
	hitStop.Update();
	camera.Update();
	map.Update();
	enemy.Update();
	player.Update();
	playerAttack.Update();
	effect.Update();
	collision.Update();
	ui.Update();

	/*シーンの終了処理*/
	if (this->IsEnd())
	{
		if (player.GetHP() < 0)
		{
			sceneChanger.ChangeScene(SceneChanger::SceneType::GAME_OVER);
		}
		else
		{
			sceneChanger.ChangeScene(SceneChanger::SceneType::GAME_CLEAR);
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void GameScene::Draw()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& map = Singleton<MapManager>		 ::GetInstance();
	auto& player = Singleton<PlayerManager>	 ::GetInstance();
	auto& playerAttack = Singleton<PlayerAttackManager>::GetInstance();
	auto& camera = Singleton<CameraManager>	 ::GetInstance();
	auto& enemy = Singleton<EnemyManager>		 ::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	auto& timer = Singleton<Timer>::GetInstance();

	/*描画*/
	camera.Draw();
	map.Draw();
	enemy.Draw();
	player.Draw();
	playerAttack.Draw();
	effect.Draw();
	ui.Draw();
	timer.Draw();
}

/// <summary>
/// シーンを終了するか
/// </summary>
bool GameScene::IsEnd()
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