//=============================================================
// @brief プレイヤー怯みアクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerKnockdown : public PlayerAction
{
public:
	 PlayerKnockdown();	//コンストラクタ
	~PlayerKnockdown();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	/*定数*/
	const float MAX_SPEED;			//最大速度
	const float DECEL;				//減速度
	const int   VIBRATION_POWER;	//振動する強さ
	const int   VIBRATION_TIME;		//振動する時間

	/*メンバ変数*/
	float nowSpeed;//現在の速度
};

