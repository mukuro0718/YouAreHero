//=============================================================
// @brief プレイヤーコンボ2アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerCombo2 : public PlayerAction
{
public:
	 PlayerCombo2();	//コンストラクタ
	~PlayerCombo2();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	int frameCount;
};

