//===========================================
// @brief パンチ攻撃コンボ３攻撃
//===========================================
#pragma once

class BossAttack;
class BossMeleeCombo3Attack : public BossAttack
{
public:
	BossMeleeCombo3Attack(const int _attackIndex);//コンストラクタ
	~BossMeleeCombo3Attack();//デストラクタ

	void Initialize() override;		//初期化
	void Update() override;		//更新
	const void Draw() const override;	//描画
private:
	int doHitCheckFrameIndex;
};

