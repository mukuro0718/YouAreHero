//===========================================
// @brief パンチコンボ２攻撃
//===========================================
#pragma once
class BossAttack;
class BossKickAttack : public BossAttack
{
public:
	BossKickAttack(const int _attackIndex);//コンストラクタ
	~BossKickAttack();//デストラクタ
		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override;	//描画
protected:
};

