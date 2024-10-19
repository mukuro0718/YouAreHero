//===========================================
// @brief 回転パンチ攻撃
//===========================================
#pragma once

class BossAttack;
class BossRotateSlashAttack : public BossAttack
{
public:
	 BossRotateSlashAttack(const int _attackIndex);	//コンストラクタ
	~BossRotateSlashAttack();						//デストラクタ

		  void Initialize() override;		//初期化
		  void Update	 () override;		//更新
	const void Draw		 () const override; //描画
};

