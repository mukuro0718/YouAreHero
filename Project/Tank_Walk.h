//=====================================================
// @brief 歩きアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_Walk : public ActionNode
{
public:
	 Tank_Walk();//コンストラクタ
	~Tank_Walk();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

