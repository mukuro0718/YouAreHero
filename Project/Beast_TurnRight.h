//=====================================================
// @brief 右に回るアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_TurnRight : public ActionNode
{
public:
	 Beast_TurnRight();//コンストラクタ
	~Beast_TurnRight();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
};

