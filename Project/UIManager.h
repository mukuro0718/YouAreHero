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

	void Initialize();		//初期化
	void Update();		//更新
	const void Draw()const;//描画
	const bool IsContinue()const;
	const bool IsDraw()const;

private:
	/*内部処理関数*/
	UIManager();//コンストラクタ

	/*メンバ変数*/
	std::vector<SceneUI*>	scene;
	int frameTime;//処理までにかかった時間
};

