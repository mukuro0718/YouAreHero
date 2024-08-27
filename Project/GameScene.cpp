#include <DxLib.h>
#include "UseSTL.h"
//#include "VECTORtoUseful.h"
#include "GameScene.h"
#include "SceneChanger.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "Debug.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CollisionManager.h"
#include "UIManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene()
	: gameState(this->TITLE)
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
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();
	
	/*初期化*/
	camera.Initialize();
	map.Initialize();
	enemy.Initialize();
	player.Initialize();
	ui.Initialize();
}

/// <summary>
/// 最終処理
/// </summary>
void GameScene::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void GameScene::Update()
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& input		  = Singleton<InputManager>		 ::GetInstance();
	auto& debug		  = Singleton<Debug>			 ::GetInstance();
	auto& camera	  = Singleton<CameraManager>	 ::GetInstance();
	auto& map		  = Singleton<MapManager>		 ::GetInstance();
	auto& player	  = Singleton<PlayerManager>	 ::GetInstance();
	auto& enemy		  = Singleton<EnemyManager>		 ::GetInstance();
	auto& collision   = Singleton<CollisionManager>	 ::GetInstance();
	auto& ui = Singleton<UIManager>::GetInstance();

	/*座標移動*/
	player.Action();
	enemy.Action();

	/*座標の補正を行う*/
	collision.HitCheck();

	/*更新処理*/
	input.Update();
	debug.Update();
	camera.Update();
	map.Update();
	enemy.Update();
	player.Update();
	ui.Update();

	/*終了処理*/
	ChangeState();
}

/// <summary>
/// 描画
/// </summary>
const void GameScene::Draw()const
{
	/*シングルトンクラスのインスタンスを取得*/
	auto& debug		  = Singleton<Debug>			 ::GetInstance();
	auto& map		  = Singleton<MapManager>		 ::GetInstance();
	auto& player	  = Singleton<PlayerManager>	 ::GetInstance();
	auto& camera	  = Singleton<CameraManager>	 ::GetInstance();
	auto& enemy		  = Singleton<EnemyManager>		 ::GetInstance();
	auto& collision	  = Singleton<CollisionManager>	 ::GetInstance();
	auto& ui		  = Singleton<UIManager>::GetInstance();

	/*描画*/
	camera.Draw();
	map.Draw();
	debug.Draw();
	enemy.Draw();
	player.Draw();
	collision.DebugDrawHit();
	ui.Draw();
}

/// <summary>
/// 終了するか
/// </summary>
void GameScene::ChangeState()
{
	/*タイトル中に何かボタンが押されたら*/
	this->gameState = this->TUTORIAL;

	/*チュートリアルがすべて終了したら*/
	this->gameState = this->GAME;

	/*ボスまたはプレイヤーのHPが０になったら*/
	this->gameState = this->RESULT;

	/*リザルトの表示が終わったら*/
	this->gameState = this->TITLE;
}
