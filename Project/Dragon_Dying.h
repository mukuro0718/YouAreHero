//=====================================================
// @brief ダイアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Dragon_Dying : public ActionNode
{
public:
	 Dragon_Dying();//コンストラクタ
	~Dragon_Dying();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//更新
};

