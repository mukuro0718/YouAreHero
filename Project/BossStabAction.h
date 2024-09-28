//=============================================
// @brief ボス突き刺し攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossStabAction : public BossAction
{
public:
	BossStabAction();
	virtual ~BossStabAction();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:
	bool isClose;//近づいたか
};

