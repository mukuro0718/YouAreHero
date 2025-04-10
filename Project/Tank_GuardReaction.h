//=====================================================
// @brief ガードリアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_GuardReaction : public ActionNode
{
public:
	 Tank_GuardReaction();//コンストラクタ
	~Tank_GuardReaction();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

