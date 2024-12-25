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
	~BossDeathAction();//デストラクタ

	void Initialize	  ()					override;//初期化
	void Update		  (Boss& _boss)			override;//更新
	void CalcParameter(const Boss& _boss)	override;//パラメーターの計算
};

