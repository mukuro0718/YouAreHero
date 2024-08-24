//========================================
// @brief マップマネージャークラス
// マップの管理を行う
//========================================
#pragma once
#include "Singleton.h"

class HP;
class UIManager : public Singleton<UIManager>
{
public:
	friend class Singleton<UIManager>;

	void Initialize();		//初期化
	void Update();		//更新
	const void Draw()const;//描画

private:
	/*内部処理関数*/
	UIManager();//コンストラクタ
	~UIManager();//デストラクタ

	/*メンバ変数*/
	HP* hp;
};

