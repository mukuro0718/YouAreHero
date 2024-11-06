//=============================================
// @brief ボススラッシュ攻撃アクションVer.2
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlash2Action : public BossAction
{
public:
	 BossSlash2Action();//コンストラクタ
	~BossSlash2Action();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//更新
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	bool isClose;//近づいたか
};

