//==========================================================
// @brief ボス休憩アクションクラス
// 何もしない
//==========================================================
#pragma once

class Boss;
class BossAction;
class BossRestAction : public BossAction
{
public:
	BossRestAction();//コンストラクタ
	virtual ~BossRestAction();//デストラクタ

	void Update(Boss& _boss)	override;//パラメータの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:

};

