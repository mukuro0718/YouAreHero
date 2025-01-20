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
	const float MAX_SPEED;	//最大速度
	const float DECEL;		//減速度

	/*メンバ変数*/
	float nowSpeed;//現在の速度
};

