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
	virtual ~BossRestAction();//デストラクタ

	void Initialize()				  override;//初期化
	void Update(Boss& _boss)	override;//パラメータの計算
	void CalcParameter(const Boss& _boss) override;//パラメーターの計算
private:
	enum class DirectionType
	{
		LEFT,
		RIGHT,
	};

	bool isSetMoveDirection;
	int directionType;
	int maxFrameCount;
};

