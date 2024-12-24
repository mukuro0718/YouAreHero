//=============================================================
// @brief プレイヤースタミナ切れアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerRunOutOfStamina : public PlayerAction
{
public:
	 PlayerRunOutOfStamina();	//コンストラクタ
	~PlayerRunOutOfStamina();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	float maxSpeed;
	float staminaConsumption;
};

