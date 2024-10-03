//==================================================
// @brief ヒットストップクラス
//==================================================
#pragma once
#include "Singleton.h"

class HitStop;
class HitStopManager : public Singleton<HitStopManager>
{
public:
	friend class Singleton<HitStopManager>;

	void Entry(HitStop& _item);
	void Exit(HitStop& _item);
	void Update();//更新
private:
	/*内部処理関数*/
	HitStopManager();//コンストラクタ
	~HitStopManager();//デストラクタ

	/*静的定数*/
	static constexpr float SLOW_MOTION_FACTOR = 0.3f;

	/*メンバ変数*/
	int type;
	int time;//ヒットストップ時間
	float slowFactor;
	std::list<HitStop*> items;
};

