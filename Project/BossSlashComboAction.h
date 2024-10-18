//=============================================
// @brief　スラッシュコンボアクションVer.1
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashComboAction : public BossAction
{
public:
	 BossSlashComboAction();//コンストラクタ
	~BossSlashComboAction();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
};

