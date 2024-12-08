//=============================================================
// @brief プレイヤー怯みアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerKnockup : public PlayerAction
{
public:
	 PlayerKnockup();	//コンストラクタ
	~PlayerKnockup();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
};

