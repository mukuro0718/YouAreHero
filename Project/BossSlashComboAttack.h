//===========================================
// @brief スラッシュコンボ攻撃Ver.1
//===========================================
#pragma once

class BossAttack;
class BossSlashComboAttack : public BossAttack
{
public:
	BossSlashComboAttack(const int _attackIndex);	//コンストラクタ
	~BossSlashComboAttack();						//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override;	//描画
private:
	int doHitCheckFrameIndex;//当たり判定を行うフレーム
};

