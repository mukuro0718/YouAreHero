//==================================================
// @brief ヒットストップクラス
//==================================================
#pragma once
#include "Singleton.h"

class HitStop : public Singleton<HitStop>
{
public:
	friend class Singleton<HitStop>;

	void Initialize();//初期化
	const void Draw()const;//描画
	bool IsHitStop();//ヒットストップが行われているか

	/*setter*/
	void SetHitStopTime(const float _time) { this->time = _time; }//ヒットストップ時間を設定
private:
	/*内部処理関数*/
	HitStop();//コンストラクタ
	~HitStop();//デストラクタ

	/*メンバ変数*/
	int time;//ヒットストップ時間
};

