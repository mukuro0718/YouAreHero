//===========================================
// @brief ジャンプ攻撃
//===========================================
#pragma once
class BossAttack;
class BossJumpAttack : public BossAttack
{
public:
	 BossJumpAttack(const int _attackIndex);//コンストラクタ
	~BossJumpAttack();						//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override;	//描画
};

