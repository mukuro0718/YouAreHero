//=============================================================
// @brief プレイヤー怯みアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerStagger : public PlayerAction
{
public:
	 PlayerStagger();	//コンストラクタ
	~PlayerStagger();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	int frameCount;
};

