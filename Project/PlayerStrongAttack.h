//=============================================================
// @brief プレイヤー強攻撃アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerStrongAttack : public PlayerAction
{
public:
	 PlayerStrongAttack();	//コンストラクタ
	~PlayerStrongAttack();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	bool isPlay;
	bool isCharge;
	int frameCount;
};

