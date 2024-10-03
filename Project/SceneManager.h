//=======================================
// @brief シーンマネージャークラス
// シーンの管理を行う
//=======================================
#pragma once
#include "Singleton.h"

class SceneBase;
class SceneManager : public Singleton<SceneManager>
{
public:
	friend class Singleton<SceneManager>;

	void Update();//更新
private:
	/*内部処理関数*/
			 SceneManager();//コンストラクタ
	virtual ~SceneManager();//デストラクタ
	void SceneChange();

	/*メンバ変数*/
	static SceneManager* instance;	//唯一のインスタンス
	SceneBase*			 mainScene;	//メインシーンインスタンス
	SceneBase*			 loadScene;	//ロードシーンインスタンス
};

