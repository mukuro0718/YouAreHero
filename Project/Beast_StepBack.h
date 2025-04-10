//=====================================================
// @brief ステップバックアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_StepBack : public ActionNode
{
public:
	 Beast_StepBack();//コンストラクタ
	~Beast_StepBack();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
};

