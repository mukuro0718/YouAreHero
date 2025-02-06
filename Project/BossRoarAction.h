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
	/*定数*/
	const int PLAY_ROAR_SOUND_PLAY_TIME;//咆哮の効果音を鳴らす再生時間

	/*メンバ変数の初期化*/
	int	  prevState;			 //前の怒り状態
	bool  isFinishedFirstRoar;	 //最初の咆哮が終了したかフラグ
	short checkedState;			 //欲求値の計算を行うときに使用する確認したい状態
};

