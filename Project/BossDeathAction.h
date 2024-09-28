//============================================
// @brief ボスデスパラメータ
//============================================
#pragma once

class Boss;
class BossAction;
class BossDeathAction : public BossAction
{
public:
	BossDeathAction();//コンストラクタ
	virtual ~BossDeathAction();//デストラクタ

	void Initialize	  ()					override;//初期化
	void Update		  (Boss& _boss)			override;//パラメータの計算
	void CalcParameter(const Boss& _boss)	override;//パラメーターの計算
private:

};

