//============================================
// @brief ボス待機パラメータ
//============================================
#pragma once

class Boss;
class BossAction;
class BossIdleAction : public BossAction
{
public:
	BossIdleAction();//コンストラクタ
	virtual ~BossIdleAction();//デストラクタ

	void Initialize()						override;//初期化
	void Update(Boss& _boss)				override;//パラメータの計算
	void CalcParameter(const Boss& _boss)	override;//パラメーターの計算
private:
	int maxFrameCount;
};

