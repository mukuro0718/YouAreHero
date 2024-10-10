//=============================================
// @brief　メレーコンボ３アクションパラメーター
//=============================================
#pragma once

class Boss;
class BossAction;
class BossMeleeCombo3Action : public BossAction
{
public:
	BossMeleeCombo3Action();
	virtual ~BossMeleeCombo3Action();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:

};

