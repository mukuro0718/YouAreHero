//===========================================
// @brief 回転蹴り攻撃
//===========================================
#pragma once

class BossAttack;
class BossMeleeAttack : public BossAttack
{
public:
	BossMeleeAttack(const int _attackIndex);//コンストラクタ
	~BossMeleeAttack();//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override;	//描画
private:
	int hitCheckCount;//当たり判定の回数
	int hitCheckInterval;//当たり判定のインターバル
};

