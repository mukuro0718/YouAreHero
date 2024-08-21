//=================================================
// @brief 画面の設定クラス
// window/fullScreenの切り替え
// カラービットの設定
// 画面サイズの設定
//=================================================

#pragma once
#include "Singleton.h"

class ScreenSetup : public Singleton<ScreenSetup>
{
public:
	friend class Singleton<ScreenSetup>;
	
	int	Initialize	();//初期化
protected:
	/*内部処理関数*/
	ScreenSetup();
	virtual ~ScreenSetup();//デストラクタ
};

