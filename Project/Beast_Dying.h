//=====================================================
// @brief デスアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Dying : public ActionNode
{
public:
	 Beast_Dying();//コンストラクタ
	~Beast_Dying();//デストラクタ

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//更新
private:
	bool  isInitialize;		//初期化したか
};

