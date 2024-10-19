//===========================================
// @brief ジャンプ攻撃
//===========================================
#pragma once

class BossAttack;
class BossStabAttack : public BossAttack
{
public:
	BossStabAttack(const int _attackIndex);//コンストラクタ
	~BossStabAttack();//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override;	//描画
};

