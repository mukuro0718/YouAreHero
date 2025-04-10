//=====================================================
// @brief リアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Reaction : public ActionNode
{
public:
	 Mage_Reaction();//コンストラクタ
	~Mage_Reaction();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

