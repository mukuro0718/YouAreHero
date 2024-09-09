#include "SceneState.h"

//SceneState* Singleton<SceneState>::instance = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
SceneState::SceneState()
	: currentScene(this->TITLE)
	, prevScene(this->TITLE)
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneState::~SceneState()
{

}


void SceneState::Update()
{
	this->prevScene = this->currentScene;
}

/// <summary>
/// シーンの変更
/// </summary>
void SceneState::ChangeTitleToTutorial()
{
	this->currentScene = this->TUTORIAL;
}
void SceneState::ChangeTutorialToGame()
{
	this->currentScene = this->GAME;
}
void SceneState::ChangeGameToGameclear()
{
	this->currentScene = this->GAMECLEAR;
}
void SceneState::ChangeGameToGameover()
{
	this->currentScene = this->GAMEOVER;
}
void SceneState::ChangeResultToTitle()
{
	this->currentScene = this->TITLE;
}

/// <summary>
/// どのシーンか
/// </summary>
const bool SceneState::IsTheCurrentStateOfTheTitle() const
{
	if (this->currentScene == this->TITLE)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheTutorial() const
{
	if (this->currentScene == this->TUTORIAL)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheGame() const
{
	if (this->currentScene == this->GAME)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheResult() const
{
	if (this->currentScene == this->MASK_RESULT)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheGameClear() const
{
	if (this->currentScene == this->GAMECLEAR)return true;
	return false;
}
const bool SceneState::IsTheCurrentStateOfTheGameOver() const
{
	if (this->currentScene == this->GAMEOVER)return true;
	return false;
}

/// <summary>
/// 現在と前のシーンが異なっているか
/// </summary>
const bool SceneState::IsDifferentState() const
{
	if (this->currentScene != this->prevScene)return true;
	return false;
}