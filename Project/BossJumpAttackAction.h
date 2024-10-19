//=============================================
// @brief ボスジャンプ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossJumpAttackAction : public BossAction
{
public:
	BossJumpAttackAction();
	virtual ~BossJumpAttackAction();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	bool isClose;//近づいたか
};

