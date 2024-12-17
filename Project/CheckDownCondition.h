//=======================================================
// @brief ダウンを確認するコンディションノード
//=======================================================
#pragma once

class ConditionNode;
class CheckDownCondition : public ConditionNode
{
public:
	 CheckDownCondition();//コンストラクタ
	~CheckDownCondition();//デストラクタ

	NodeState Update() override;//更新

private:
	/*メンバ変数*/
	
};

