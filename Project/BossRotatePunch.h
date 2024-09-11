//===========================================
// @brief 回転パンチ攻撃
//===========================================
#pragma once
class BossAttack;
class BossRotatePunch : public BossAttack
{
public:
	BossRotatePunch(const int _attackIndex);//コンストラクタ
	~BossRotatePunch();//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override; //描画
protected:
};

