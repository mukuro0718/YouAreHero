//==========================================================
// @brief ボス休憩アクションクラス
// 何もしない
//==========================================================
#pragma once

class Boss;
class BossAction;
class BossRestAction : public BossAction
{
public:
	 BossRestAction();//コンストラクタ
	~BossRestAction();//デストラクタ

	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//更新
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	int nowAnimationType;
	int  maxFrameCount;			//最大フレーム
	bool isChangeColorScale;	//カラースケールを初期化したか
	int	 prevAngryState;		//前の怒り状態
	bool isInitializeColorScale;//カラースケールを初期化したか
};

