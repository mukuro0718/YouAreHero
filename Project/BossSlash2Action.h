//=============================================
// @brief ボススラッシュ攻撃アクションVer.2
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlash2Action : public BossAction
{
public:
	BossSlash2Action();
	virtual ~BossSlash2Action();

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメーターの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:

};

