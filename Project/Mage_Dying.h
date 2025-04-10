//=====================================================
// @brief デスアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Dying : public ActionNode
{
public:
	 Mage_Dying();//コンストラクタ
	~Mage_Dying();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

