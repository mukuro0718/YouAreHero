//=====================================================
// @brief ステップバックアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_StepBack : public ActionNode
{
public:
	 Beast_StepBack();//コンストラクタ
	~Beast_StepBack();//デストラクタ

	NodeState Update()override;//更新
};

