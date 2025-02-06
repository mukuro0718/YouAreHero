//=============================================
// @brief ボス突き刺し攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossStabAction : public BossAction
{
public:
	 BossStabAction();//コンストラクタ
	~BossStabAction();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//更新
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:
	/*定数*/
	const int	 ATTACK_TYPE;		 //攻撃の種類
	const int	 HIT_STOP_DELAY;	 //ヒットストップディレイ
	const float	 HIT_STOP_TIME;		 //ヒットストップ時間
	const int	 HIT_STOP_TYPE;		 //ヒットストップの種類
	const float	 SLOW_FACTOR;		 //スロー係数
	const short	 ROTATE_FIX_COUNT;	 //回転補正用
	const short	 ROTATE_PLAY_TIME;	 //回転補正用
	const short	 MOVE_PLAY_TIME;	 //移動用
	const short	 STOP_PLAY_TIME;	 //停止用
	const float	 STOP_DISTANCE;		 //停止する距離
	const VECTOR ROTATE_LERP_VALUE;	 //回転補正量
	const short  MAX_INTERVAL;		 //最大インターバル
	const float	 SLOW_PLAY_TIME;	 //スロー中の再生時間
	const float	 MIN_ACTION_DISTANCE;//アクションが可能になる距離
	const float	 MAX_ACTION_DISTANCE;//アクションが可能になる距離
	const short	 CHECK_STATE;		 //どの状態で確認するか
	const float	 SLOW_LIMIT_TIME;	 //アニメーションを遅くする最大時間

	/*メンバ変数*/
	bool	isClose;			//近づいたか
	float	speed;				//速度
};

