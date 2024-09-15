//=============================================
// @brief ボス回転パンチ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossRotatePunchAction : public BossAction
{
public:
	BossRotatePunchAction();
	virtual ~BossRotatePunchAction();

	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	bool isClose;//近づいたか
};

