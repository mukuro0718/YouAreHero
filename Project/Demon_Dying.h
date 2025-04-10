//=====================================================
// @brief デスアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Demon_Dying : public ActionNode
{
public:
	 Demon_Dying();//コンストラクタ
	~Demon_Dying();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

