//===========================================
// @brief スラッシュ攻撃
//===========================================
#pragma once
class BossAttack;
class BossFlyAttack : public BossAttack
{
public:
	BossFlyAttack(const int _attackIndex);//コンストラクタ
	~BossFlyAttack();//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override;	//描画
protected:
};

