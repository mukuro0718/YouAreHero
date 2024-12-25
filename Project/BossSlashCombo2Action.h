//=============================================
// @brief ボス回転蹴り攻撃アクション
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashCombo2Action : public BossAction
{
public:
	 BossSlashCombo2Action();//コンストラクタ
	~BossSlashCombo2Action();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//更新
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算

private:
	bool	isClose;//一度でも近づいたか
	float	targetOffset;
	float	hitStopTime;
	int		hitStopDelay;
	int		hitStopType;
	float	slowFactor;
	VECTOR	rotateLerpValue;	//回転補正量
	short	rotateFixFrame;		//回転補正用
	short	moveFrame;			//移動用
	short	rotateFrame1;		//回転用
	short	rotateFrame2;		//回転用
	float	stopDistance;		//停止する距離
	float	speed;				//速度
	short	maxInterval;		//最大インターバル
	short	checkState;			//どの状態で確認するか
	float	actionDistance;		//アクションが可能になる距離
	short   normalDisireValue;
};

