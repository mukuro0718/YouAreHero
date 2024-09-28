//=============================================
// @brief ボス回転蹴り攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossHurricaneKickAction : public BossAction
{
public:
	BossHurricaneKickAction();
	virtual ~BossHurricaneKickAction();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:

};

