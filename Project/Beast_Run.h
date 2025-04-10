//=====================================================
// @brief ダッシュアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Run : public ActionNode
{
public:
	 Beast_Run();//コンストラクタ
	~Beast_Run();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
};

