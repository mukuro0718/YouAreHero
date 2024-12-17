//=============================================================
// @brief プレイヤーコンボ１アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerCombo1 : public PlayerAction
{
public:
	 PlayerCombo1();	//コンストラクタ
	~PlayerCombo1();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	int frameCount;
};

