//=============================================================
// @brief プレイヤーガード怯みアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerBlockStagger : public PlayerAction
{
public:
	 PlayerBlockStagger();	//コンストラクタ
	~PlayerBlockStagger();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	float staminaConsumption;
	short cancelableFrame;
};

