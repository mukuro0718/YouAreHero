//=============================================================
// @brief プレイヤーデスアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerDeath : public PlayerAction
{
public:
	 PlayerDeath();	//コンストラクタ
	~PlayerDeath();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
};

