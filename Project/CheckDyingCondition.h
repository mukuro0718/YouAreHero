//=======================================================
// @brief HPを確認するコンディションノード
//=======================================================
#pragma once

class ConditionNode;
class CheckDyingCondition : public ConditionNode
{
public:
	 CheckDyingCondition();//コンストラクタ
	~CheckDyingCondition();//デストラクタ

	NodeState Update() override;//更新

private:
	/*メンバ変数*/
	
};

