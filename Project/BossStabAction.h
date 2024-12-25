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
	bool	isClose;			//近づいたか
	float	hitStopTime;		//ヒットストップ時間
	int		hitStopDelay;		//ヒットストップディレイ
	int		hitStopType;		//ヒットストップの種類
	float	slowFactor;			//スロー係数
	short	rotateFixFrame;		//回転補正用
	short	moveFrame;			//移動用
	short	stopFrame;			//停止用
	short	rotateFrame;		//回転用
	short	homingFrame;		//追跡用
	float	stopDistance;		//停止する距離
	VECTOR	rotateLerpValue;	//回転補正量
	float	speed;				//速度
	float	slowPlayTime;		//スロー中の再生時間
	short	maxInterval;		//最大インターバル
	float	actionDistance;		//アクションが可能になる距離
	short	normalDisireValue;	//最大欲求値
	short	checkState;			//どの状態で確認するか
};

