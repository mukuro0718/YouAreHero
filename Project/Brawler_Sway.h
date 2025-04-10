//=====================================================
// @brief 回避アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_Sway : public ActionNode
{
public:
	 Brawler_Sway();//コンストラクタ
	~Brawler_Sway();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

