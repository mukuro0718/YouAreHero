//=============================================
// @brief ボススラッシュ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashAction : public BossAction
{
public:
	 BossSlashAction();//コンストラクタ
	~BossSlashAction();//デストラクタ

	void Initialize		()					override;//初期化
	void Update			(Boss& _boss)		override;//更新
	void CalcParameter	(const Boss& _boss) override;//パラメーターの計算
private:
	bool isClose;//近づいたか
};

