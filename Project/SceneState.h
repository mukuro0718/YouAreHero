//===============================================
// @brief シーンチェンジャークラス
// シーンタイプの変更関数を実装
// シーンのタイプを保持
// シングルトンで実装
//===============================================
#pragma once
#include "Singleton.h"

class SceneState :public Singleton<SceneState>
{
public:
	friend class Singleton<SceneState>;

	void Update();

	/*シーンの変更*/
	void ChangeTitleToTutorial();
	void ChangeTutorialToGame();
	void ChangeGameToGameover();
	void ChangeGameToGameclear();
	void ChangeResultToTitle();

	/*シーンの判定*/
	const bool IsTheCurrentStateOfTheTitle	  () const;
	const bool IsTheCurrentStateOfTheTutorial () const;
	const bool IsTheCurrentStateOfTheGame	  () const;
	const bool IsTheCurrentStateOfTheResult   () const;
	const bool IsTheCurrentStateOfTheGameOver () const;
	const bool IsTheCurrentStateOfTheGameClear() const;

	/*現在と前のシーンが異なっているか*/
	const bool IsDifferentState()const;
private:
	/*静的定数*/
	static constexpr unsigned int TITLE		  = (1 << 0);
	static constexpr unsigned int TUTORIAL	  = (1 << 1);
	static constexpr unsigned int GAME		  = (1 << 2);
	static constexpr unsigned int GAMEOVER	  = (1 << 3);
	static constexpr unsigned int GAMECLEAR   = (1 << 4);
	static constexpr unsigned int MASK_RESULT = GAMECLEAR | GAMEOVER;

	/*内部処理関数*/
	SceneState();//コンストラクタ
	virtual ~SceneState();//デストラクタ

	/*メンバ変数*/
	unsigned int currentScene;		//現在のシーン
	unsigned int prevScene;
};