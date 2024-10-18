//===========================================
// @brief パンチ攻撃
//===========================================
#pragma once

class BossAttack;
class BossPunchAttack : public BossAttack
{
public:
	BossPunchAttack(const int _attackIndex);//コンストラクタ
	~BossPunchAttack();//デストラクタ

	void Initialize() override;		//初期化
	void Update() override;		//更新
	const void Draw() const override;	//描画
};

