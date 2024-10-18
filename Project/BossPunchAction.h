//=============================================
// @brief ボスパンチ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossPunchAction : public BossAction
{
public:
	BossPunchAction();
	virtual ~BossPunchAction();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:

};

