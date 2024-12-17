//===========================================================
// @brief レベルを上げる咆哮の判定を行うコンディションノード
//===========================================================
#pragma once

class ConditionNode;
class CheckRaiseLevelCondition : public ConditionNode
{
public:
	 CheckRaiseLevelCondition();//コンストラクタ
	~CheckRaiseLevelCondition();//デストラクタ

	NodeState Update	() override;//更新
};

