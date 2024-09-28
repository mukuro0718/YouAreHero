#include <DxLib.h>
#include "SceneManager.h"
#include "SceneBase.h"
#include "GameScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "TitleScene.h"
#include "TutorialScene.h"
#include "LoadScene.h"
#include "SceneChanger.h"
#include "FPSController.h"

//SceneManager* Singleton<SceneManager>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
SceneManager::SceneManager()
	: mainScene(nullptr)
	, loadScene(nullptr)
	, fps	   (nullptr)
{
	/*インスタンスの作成*/
	this->mainScene = new TitleScene   ();//メインシーン
	this->loadScene = new LoadScene	   ();//ロードシーン
	this->fps		= new FPSController();//FPSコントローラー
}
/// <summary>
/// デストラクタ
/// </summary>
SceneManager::~SceneManager()
{

}
/// <summary>
/// 更新
/// </summary>
void SceneManager::Update()
{
	if (GetASyncLoadNum() == 0)
	{
		/*画面をきれいにする*/
		ClearDrawScreen();
		clsDx();

		/*シーンの更新*/
		this->mainScene->Update();

		/*シーンの描画*/
		this->fps->Draw();
		this->mainScene->Draw();
		
		/*シーンの変更*/
		SceneChange();

		/*ＦＰＳの平均を算出*/
		this->fps->Average();
		
		/*裏画面の内容を表に反映させる*/
		ScreenFlip();
		
		/*ＦＰＳの処理*/
		this->fps->Wait();
	}
	else
	{
		this->loadScene->Update();
		this->loadScene->Draw();
	}
}


void SceneManager::SceneChange()
{
	auto& changer = SceneChanger::GetInstance();

	if (changer.GetNowSceneType() != changer.GetNextSceneType())
	{
		switch (changer.GetNextSceneType())
		{
		case SceneChanger::SceneType::TITLE:
			delete(this->mainScene);
			this->mainScene = new TitleScene();
			break;
		case SceneChanger::SceneType::TUTORIAL:
			delete(this->mainScene);
			this->mainScene = new TutorialScene();
			break;
		case SceneChanger::SceneType::GAME:
			delete(this->mainScene);
			this->mainScene = new GameScene();
			break;
		case SceneChanger::SceneType::GAME_CLEAR:
			delete(this->mainScene);
			this->mainScene = new GameClearScene();
			break;
		case SceneChanger::SceneType::GAME_OVER:
			delete(this->mainScene);
			this->mainScene = new GameOverScene();
			break;
		}
		changer.SynchroScene();
	}
}