//=====================================================
// @brief 待機アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Beast_Idle : public ActionNode
{
public:
	 Beast_Idle();//コンストラクタ
	~Beast_Idle();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
};

