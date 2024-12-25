//=============================================
// @brief ボスジャンプ攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossJumpAttackAction : public BossAction
{
public:
	 BossJumpAttackAction();//コンストラクタ
	~BossJumpAttackAction();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//更新
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	bool	isClose;			//近づいたか
	float	hitStopTime;		//ヒットストップ時間
	int		hitStopDelay;		//ヒットストップディレイ
	int		hitStopType;		//ヒットストップの種類
	float	slowFactor;			//スロー係数
	VECTOR	rotateLerpValue;	//回転補正量
	float	targetOffset;		//目標補正
	short	rotateFixFrame;		//回転補正用
	short	moveFrame;			//移動用
	short	rotateFrame;		//回転用
	float	stopDistance;		//停止する距離
	float	speed;				//速度
	short	maxInterval;		//最大インターバル
	short	checkState;			//どの状態で確認するか
	float	actionDistance;		//アクションが可能になる距離
};

