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
	int nowAnimationType;		//今再生しているアニメーション
	int nextAnimation2;			//次のアニメーション２
	int  maxFrameCount;			//最大フレーム
	bool isChangeColorScale;	//カラースケールを初期化したか
	int	 prevState;				//前の状態
	bool isInitializeColorScale;//カラースケールを初期化したか
	bool isDownUp;				//ダウンから起き上がっているか
	short checkedState;
};

