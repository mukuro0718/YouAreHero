#include <DxLib.h>
#include "GoriLib.h"
#include "UseSTL.h"
#include "GameObjectTag.h"
#include "GameScene.h"
#include "SceneChanger.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "Debug.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "UIManager.h"

using namespace GoriLib;

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene()
	: gameState(this->TITLE)
{
	this->physics = new GoriLib::Physics;
	Initialize();
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
	delete(this->physics);
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
	map.Initialize(this->physics);
	enemy.Initialize(this->physics);
	player.Initialize(this->physics);
	ui.Initialize();
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
	map.Finalize(this->physics);
	enemy.Finalize(this->physics);
	player.Finalize(this->physics);
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
	auto& ui = Singleton<UIManager>::GetInstance();

	/*更新処理*/
	input.Update();
	debug.Update();
	camera.Update();
	map.Update(this->physics);
	enemy.Update(this->physics);
	player.Update(this->physics);
	ui.Update();
	this->physics->Update();
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
	auto& ui		  = Singleton<UIManager>::GetInstance();

	/*描画*/
	camera.Draw();
	map.Draw();
	debug.Draw();
	enemy.Draw();
	player.Draw();
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
