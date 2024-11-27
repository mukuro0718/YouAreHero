//=============================================================
// @brief プレイヤーダッシュアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerBlock : public PlayerAction
{
public:
	 PlayerBlock();	//コンストラクタ
	~PlayerBlock();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
};

