//=======================================================
// @brief 怯み値が定数以上か
//=======================================================
#pragma once

class ConditionNode;
class Condition_IsFrighteningValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsFrighteningValueGreaterThanConstant(const int _specifiedFrighteningValue);//コンストラクタ
	~Condition_IsFrighteningValueGreaterThanConstant();//デストラクタ

	NodeState Update() override;//更新
private:
	const int SPECIFIED_FRIGHTENING_VALUE;//怯み値
};

