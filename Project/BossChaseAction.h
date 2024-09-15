//=============================================
// @brief ボス追跡アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossChaseAction : public BossAction
{
public:
	BossChaseAction();
	virtual ~BossChaseAction();

	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	
};

