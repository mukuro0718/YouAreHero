//=============================================================
// @brief プレイヤー回復アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerHeal : public PlayerAction
{
public:
	 PlayerHeal();	//コンストラクタ
	~PlayerHeal();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	int healValue;
	int maxHp;
};

