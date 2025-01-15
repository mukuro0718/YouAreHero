//============================================
// @brief ボス待機パラメータ
//============================================
#pragma once

class Boss;
class BossAction;
class BossIdleAction : public BossAction
{
public:
	 BossIdleAction();//コンストラクタ
	~BossIdleAction();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//更新
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	/*静的定数*/
	static constexpr int LEFT = 0;
	static constexpr int RIGHT = 1;
	const int NORMAL_MAX_ANGRY_FRAME_COUNT;
	const int ANGRY_MAX_ANGRY_FRAME_COUNT;

	/*メンバ変数*/
	bool	isSetMoveDirection;	//向きをセットしたか
	int		directionType;			//向きの種類
	int		maxFrameCount;			//フレームカウント
	bool	isChangeColorScale;	//カラースケールを変更するか
	int		prevState;				//前の状態
	bool	isInitializeColorScale;//カラースケールを初期化したか
	float	speed;
	VECTOR	rotateLerpValue;
	short	nextAnimation2;
	int		maxInterval;
	short	checkedState;
};

