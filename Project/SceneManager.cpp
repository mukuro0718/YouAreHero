#include <DxLib.h>
#include "GoriLib.h"
#include "SceneManager.h"
#include "GameScene.h"
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
	this->mainScene = new GameScene	   ();//メインシーン
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
		/*インプットマネージャーのインスタンスを取得*/
		//auto& input = InputManager::GetInstance();

		/*画面をきれいにする*/
		ClearDrawScreen();
		clsDx();

		/*インプットマネージャーの更新*/
		//input.Update();

		/*シーンの更新*/
		this->mainScene->Update();

		/*シーンの描画*/
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
/// <summary>
/// シーンの変更
/// </summary>
void SceneManager::SceneChange()
{
	/*シーンチェンジャークラスのインスタンスを取得*/
	auto& changer = SceneChanger::GetInstance();

	/*もし現在のシーンタイプと前のシーンタイプが異なっていたら処理を行う*/
	if (changer.GetNowSceneType() != changer.GetNextSceneType())
	{
		//シーンタイプによって作成するインスタンスを変える
		switch (changer.GetNextSceneType())
		{
		case SceneChanger::SceneType::GAME:
			delete(this->mainScene);
			this->mainScene = new GameScene();
			break;
		}
		//シーンを同期させる
		changer.SynchroScene();
	}
}
