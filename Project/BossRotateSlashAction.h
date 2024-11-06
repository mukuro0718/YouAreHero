//=============================================
// @brief ボス回転スラッシュ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossRotateSlashAction : public BossAction
{
public:
	 BossRotateSlashAction();//コンストラクタ
	~BossRotateSlashAction();//デストラクタ

	void Initialize		()					override;//初期化
	void Update			(Boss& _boss)		override;//更新
	void CalcParameter	(const Boss& _boss)	override;//パラメーターの計算
private:
	bool isClose;//近づいたか
};

