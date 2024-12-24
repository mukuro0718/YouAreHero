//=============================================================
// @brief プレイヤー待機アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerIdle : public PlayerAction
{
public:
	 PlayerIdle();	//コンストラクタ
	~PlayerIdle();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
};

