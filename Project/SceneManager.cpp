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
{
	/*インスタンスの作成*/
	this->mainScene = new TitleScene   ();//メインシーン
	this->loadScene = new LoadScene	   ();//ロードシーン
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
		auto& fps = Singleton<FPSController>::GetInstance();

		fps.CalcStartTime();

		/*画面をきれいにする*/
		ClearDrawScreen();
		clsDx();

		/*シーンの更新*/
		this->mainScene->Update();

		/*FPSの描画*/
		fps.Draw();

		/*シーンの描画*/
		this->mainScene->Draw();
		
		/*シーンの変更*/
		SceneChange();

		/*裏画面の内容を表に反映させる*/
		ScreenFlip();
		
		/*ＦＰＳの平均を算出*/
		fps.Update();
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