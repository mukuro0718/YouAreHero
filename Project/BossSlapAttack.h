//===========================================
// @brief スラップ攻撃
//===========================================
#pragma once

class BossAttack;
class BossSlapAttack : public BossAttack
{
public:
	BossSlapAttack(const int _attackIndex);//コンストラクタ
	~BossSlapAttack();//デストラクタ

	void Initialize() override;		//初期化
	void Update() override;		//更新
	const void Draw() const override;	//描画
};

