//========================================
// @brief マップマネージャークラス
// マップの管理を行う
//========================================
#pragma once
#include "Singleton.h"

class SceneUI;
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
private:
	/*内部処理関数*/
	UIManager();//コンストラクタ

	/*メンバ変数*/
	SceneUI* scene;
	int		 frameTime;//処理までにかかった時間
};

