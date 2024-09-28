//===============================================
// @brief シーンチェンジャークラス
// シーンタイプの変更関数を実装
// シーンのタイプを保持
// シングルトンで実装
//===============================================
#pragma once
#include "Singleton.h"

class SceneChanger :public Singleton<SceneChanger>
{
public:
	friend class Singleton<SceneChanger>;

	//シーンタイプ
	enum class SceneType
	{
		TITLE		= 0,
		TUTORIAL	= 1,
		GAME		= 2,//ゲーム
		GAME_CLEAR	= 3,
		GAME_OVER	= 4,
	};

	void SynchroScene();							//シーンの同期
	void ChangeScene (const SceneType _nextScene);	//シーンの変更
	
	/*getter*/
	const SceneType GetNowSceneType ()const { return this->nowScene; }		//現在のシーンタイプのgetter
	const SceneType GetNextSceneType()const { return this->nextSceneType; } //次のシーンタイプのgetter
private:
	/*内部処理関数*/
			 SceneChanger();//コンストラクタ
	virtual ~SceneChanger();//デストラクタ

	/*メンバ変数*/
	SceneType			 nowScene;		//現在のシーン
	SceneType			 nextSceneType;	//次のシーン
};