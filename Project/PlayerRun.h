//=============================================================
// @brief プレイヤーダッシュアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerRun : public PlayerAction
{
public:
	 PlayerRun();	//コンストラクタ
	~PlayerRun();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
};

