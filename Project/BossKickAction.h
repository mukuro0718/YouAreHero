//=============================================
// @brief ボス飛び攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossKickAction : public BossAction
{
public:
	BossKickAction();
	virtual ~BossKickAction();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
};

