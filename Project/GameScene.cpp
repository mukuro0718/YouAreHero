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
#include "Character.h"
#include "Player.h"
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
#include "SoundManager.h"

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
	auto& sound = Singleton<SoundManager>::GetInstance();

	/*初期化*/
	camera.Initialize();
	timer.Initialize();
	map.Initialize();
	player.Initialize();
	enemy.Initialize();
	playerAttack.Initialize();
	ui.Initialize();
	debug.Initialize();
	effect.Initialize();
	sound.Initialize();

	/*BGMの再生*/
	sound.OnIsPlayBgm(SoundManager::BgmType::BATTLE_BGM);
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
	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		timer.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("TIMER:%d\n", frameTime);
	}
	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		debug.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DEBUG:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		input.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("INPUT:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		hitStop.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("HITSTOP:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		camera.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("CAMERA:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		map.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("MAP:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		enemy.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("ENEMY:%d\n", frameTime);
	}


	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		player.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("PLAYER:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		effect.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("EFFECT:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		collision.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("COLL:%d\n", frameTime);
	}
	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		ui.Update();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("UI:%d\n", frameTime);
	}

	/*シーンの終了処理*/
	if (IsEnd())
	{
		auto& sound = Singleton<SoundManager>::GetInstance();
		sound.Initialize();
		sound.OnIsPlayBgm(SoundManager::BgmType::TITLE_BGM);
		sceneChanger.ChangeScene(SceneChanger::SceneType::SELECT);
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
	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		camera.Draw();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DRAW_CAMERA:%d\n", frameTime);
	}
	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		map.Draw();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DRAW_MAP:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		enemy.Draw();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DRAW_ENEMY:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		player.Draw();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DRAW_PLAYER:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		playerAttack.Draw();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DRAW_PLAYER_ATTACK:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		effect.Draw();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DRAW_EFFECT:%d\n", frameTime);
	}

	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		ui.Draw();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DRAW_UI:%d\n", frameTime);
	}
	{
		//LONGLONG startTime = GetNowHiPerformanceCount();
		timer.Draw();
		//LONGLONG endTime = GetNowHiPerformanceCount();
		//LONGLONG frameTime = endTime - startTime;
		//printfDx("DRAW_TIMER:%d\n", frameTime);
	}

	//printfDx("GAME_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// シーンを終了するか
/// </summary>
bool GameScene::IsEnd()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& ui = Singleton<UIManager>::GetInstance();

	/*ゲームシーンが終了可能だったらtrueを返す*/
	if (ui.IsDraw())
	{
		return true;
	}
	return false;
}