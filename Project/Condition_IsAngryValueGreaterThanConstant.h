//=======================================================
// @brief 怒り値を確認するコンディションノード
//=======================================================
#pragma once

class ConditionNode;
class Condition_IsAngryValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsAngryValueGreaterThanConstant(const int _specifiedAngryValue);//コンストラクタ
	~Condition_IsAngryValueGreaterThanConstant();//デストラクタ

	NodeState Update() override;//更新

private:
	const int SPECIFIED_ANGRY_VALUE;//怒り値

};

