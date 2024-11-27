//=============================================================
// @brief プレイヤー歩きアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerWalk : public PlayerAction
{
public:
	 PlayerWalk();	//コンストラクタ
	~PlayerWalk();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
};

