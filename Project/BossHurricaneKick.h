//===========================================
// @brief 回転蹴り攻撃
//===========================================
#pragma once

class BossAttack;
class BossHurricaneKick : public BossAttack
{
public:
	BossHurricaneKick(const int _attackIndex);//コンストラクタ
	~BossHurricaneKick();//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override;	//描画
protected:
};

