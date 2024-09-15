//=============================================
// @brief ボススラッシュ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashAction : public BossAction
{
public:
	BossSlashAction();
	virtual ~BossSlashAction();

	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:
};

