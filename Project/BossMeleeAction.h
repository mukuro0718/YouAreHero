//=============================================
// @brief ボス回転蹴り攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossMeleeAction : public BossAction
{
public:
	BossMeleeAction();
	virtual ~BossMeleeAction();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:

};

