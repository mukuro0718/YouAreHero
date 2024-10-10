//===========================================
// @brief 回転パンチ攻撃
//===========================================
#pragma once
class BossAttack;
class BossRotatePunchAttack : public BossAttack
{
public:
	BossRotatePunchAttack(const int _attackIndex);//コンストラクタ
	~BossRotatePunchAttack();//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override; //描画
protected:
};

