//=======================================================
// @brief ダウンを確認するコンディションノード
//=======================================================
#pragma once

class ConditionNode;
class Condition_IsDownValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsDownValueGreaterThanConstant(const int _specifiedDownValue);//コンストラクタ
	~Condition_IsDownValueGreaterThanConstant();//デストラクタ

	NodeState Update() override;//更新

private:
	const int SPECIFIED_DOWN_VALUE;//ダウン値

};

