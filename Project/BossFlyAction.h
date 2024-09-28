//=============================================
// @brief ボス飛び攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossFlyAction : public BossAction
{
public:
	BossFlyAction();
	virtual ~BossFlyAction();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:

};

