//============================================
// @brief ボス咆哮パラメータ
//============================================
#pragma once

class Boss;
class BossAction;
class BossRoarAction : public BossAction
{
public:
	 BossRoarAction();//コンストラクタ
	~BossRoarAction();//デストラクタ
	
	void Initialize	  ()				  override;//初期化
	void Update		  (Boss& _boss)		  override;//パラメータの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	static constexpr char FLAG_YES = 1 << 0;
	int	 prevState;		//前の怒り状態
	char isFinishedFirstRoar;	//最初の咆哮が終了したかフラグ
	char isInitializeColorScale;//カラースケールを初期化したか
};

