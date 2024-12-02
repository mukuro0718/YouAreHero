//=============================================================
// @brief プレイヤーコンボ3アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerCombo3 : public PlayerAction
{
public:
	 PlayerCombo3();	//コンストラクタ
	~PlayerCombo3();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	bool isPlay;
};

