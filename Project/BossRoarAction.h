//============================================
// @brief ボス咆哮パラメータ
//============================================
#pragma once

class Boss;
class BossAction;
class BossRoarAction : public BossAction
{
public:
	BossRoarAction();//コンストラクタ
	virtual ~BossRoarAction();//デストラクタ

	void Update(Boss& _boss)	override;//パラメータの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:

};

