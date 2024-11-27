//=============================================================
// @brief プレイヤーダッシュアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerAvoid : public PlayerAction
{
public:
	 PlayerAvoid();	//コンストラクタ
	~PlayerAvoid();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	bool isPlay;//このアクションを行っているか
	int frameCount;
};

