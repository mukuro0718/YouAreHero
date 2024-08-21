#include "SceneChanger.h"

//SceneChanger* Singleton<SceneChanger>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
SceneChanger::SceneChanger()
	:nowScene(SceneType::GAME)
	, nextSceneType(nowScene)
{

}

/// <summary>
/// デストラクタ
/// </summary>
SceneChanger::~SceneChanger()
{

}

/// <summary>
/// シーンの同期
/// </summary>
void SceneChanger::SynchroScene()
{
	this->nowScene = this->nextSceneType;
}

/// <summary>
/// シーンの変更
/// </summary>
void SceneChanger::ChangeScene(const SceneType _nextScene)
{
	this->nextSceneType = _nextScene;
}
