//=============================================================
// @brief プレイヤー回復アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class PlayerHeal : public PlayerAction
{
public:
	 PlayerHeal();	//コンストラクタ
	~PlayerHeal();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	/*定数*/
	const int HEAL_VALUE;		//回復量
	const int MAX_HP;			//最大HP
	const int DO_HEAL_PLAY_TIME;//回復するフレーム時間
	const int MAX_HEAL_VALUE;	//回復するフレーム時間

	/*メンバ変数*/
	int nowTotalHealValue;//現在の総回復量
};

