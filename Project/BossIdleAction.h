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
	/*列挙体*/
	enum class DirectionType//向き
	{
		LEFT = 0,
		RIGHT = 1,
	};

	/*メンバ変数*/
	bool isSetMoveDirection;	//向きをセットしたか
	int  directionType;			//向きの種類
	int  maxFrameCount;			//フレームカウント
	bool isChangeColorScale;	//カラースケールを変更するか
	int	 prevAngryState;		//前の怒り状態
	bool isInitializeColorScale;//カラースケールを初期化したか
};

