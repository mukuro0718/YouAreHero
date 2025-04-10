//=====================================================
// @brief 歩きアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Walk : public ActionNode
{
public:
	 Mage_Walk();//コンストラクタ
	~Mage_Walk();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

