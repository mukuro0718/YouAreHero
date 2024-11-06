//==================================================
// @brief ヒットストップマネージャークラス
//==================================================
#pragma once
#include "Singleton.h"

class HitStop;
class HitStopManager : public Singleton<HitStopManager>
{
public:
	/*HitStopManagerを友達にする*/
	friend class Singleton<HitStopManager>;

	virtual ~HitStopManager();//デストラクタ

	void Entry	(HitStop& _item);//登録
	void Exit	(HitStop& _item);//解除
	void Update	();				 //更新
private:
	/*内部処理関数*/
	 HitStopManager();//コンストラクタ

	 /*メンバ変数*/
	std::list<HitStop*> items;//ヒットストップクラスのリスト
};

