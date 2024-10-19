//===========================================
// @brief スラッシュ攻撃Ver.2
//===========================================
#pragma once

class BossAttack;
class BossSlash2Attack : public BossAttack
{
public:
	BossSlash2Attack(const int _attackIndex);//コンストラクタ
	~BossSlash2Attack();//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override;	//描画
};

