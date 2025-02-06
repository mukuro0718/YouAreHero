//=============================================================
// @brief プレイヤー強攻撃アクションクラス
//=============================================================
#pragma once

class PlayerAction;
class AttackSphereColliderData;
class PlayerStrongAttack : public PlayerAction
{
public:
	 PlayerStrongAttack();	//コンストラクタ
	~PlayerStrongAttack();	//デストラクタ
	void Initialize	() override;				//初期化
	void Finalize	() override;				//後処理
	void Update		(Player& _player) override;	//更新
private:
	/*定数*/
	const float FIRST_PLAY_TIME;			//最初のアニメーション再生時間
	const float CANCELABLE_PLAY_TIME;		//キャンセル可能再生時間
	const float START_HIT_CHECK_PLAY_TIME;	//当たり判定開始再生時間
	const float END_HIT_CHECK_PLAY_TIME;	//当たり判定再生時間
	const float	POSITION_OFFSET;			//座標オフセット
	const float	Y_OFFSET;					//Yオフセット
	const int	HIT_STOP_TIME;				//ヒットストップ時間
	const int	HIT_STOP_DELAY;				//ヒットストップディレイ
	const int	HIT_STOP_TYPE;				//ヒットストップの種類
	const float	SLOW_FACTOR;				//スロー係数
	const float	STAMINA_CONSUMPTION;		//スタミナ消費量

	/*メンバ変数*/
	AttackSphereColliderData*	collider;		//コライダー
	VECTOR						firstDirection;	//最初の向き
	bool						isStartHitCheck;//あたり判定が一度でも開始したか
};

