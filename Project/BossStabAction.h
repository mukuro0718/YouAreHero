//=============================================
// @brief ボス突き刺し攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossStabAction : public BossAction
{
public:
	 BossStabAction();//コンストラクタ
	~BossStabAction();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//更新
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:
	bool isClose;//近づいたか
};

