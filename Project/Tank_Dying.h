//=====================================================
// @brief デスアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_Dying : public ActionNode
{
public:
	 Tank_Dying();//コンストラクタ
	~Tank_Dying();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

