//========================================
// @brief マップマネージャークラス
// マップの管理を行う
//========================================
#pragma once
#include "Singleton.h"

class SceneUI;
class Alert;
class UIManager : public Singleton<UIManager>
{
public:
	friend class Singleton<UIManager>;
	virtual ~UIManager();//デストラクタ

		  void Initialize();	 //初期化
		  void Update	 ();	 //更新
	const void Draw		 ()const;//描画
	const bool IsDraw	 ()const;//描画しているか
	const bool GetIsBackTitle()const;
		  void OnIsCallAlert();//警告が呼ばれたか
private:
	/*内部処理関数*/
	UIManager();//コンストラクタ

	/*静的定数*/
	static constexpr int SELECT_UI_INDEX = 1;

	/*メンバ変数*/
	Alert*			 alert;			//警告クラス
	vector<SceneUI*> scene;			//シーン
	int				 sceneType;		//シーンの種類
	int				 frameTime;		//処理までにかかった時間
};

