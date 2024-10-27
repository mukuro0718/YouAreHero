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
	virtual ~BossIdleAction();//デストラクタ

	void Initialize()						override;//初期化
	void Update(Boss& _boss)				override;//パラメータの計算
	void CalcParameter(const Boss& _boss)	override;//パラメーターの計算
private:
	/*列挙体*/
	enum class DirectionType//向き
	{
		LEFT = 0,
		RIGHT = 1,
	};

	/*メンバ変数*/
	bool isSetMoveDirection;//向きをセットしたか
	int  directionType;		//向きの種類
	int  maxFrameCount;		//フレームカウント
};

