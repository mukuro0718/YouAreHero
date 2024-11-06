//=============================================
// @brief ボスジャンプ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossJumpAttackAction : public BossAction
{
public:
	 BossJumpAttackAction();//コンストラクタ
	~BossJumpAttackAction();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//更新
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	bool isClose;//近づいたか
};

