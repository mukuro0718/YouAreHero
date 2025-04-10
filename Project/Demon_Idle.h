//=====================================================
// @brief 待機アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Demon_Idle : public ActionNode
{
public:
	 Demon_Idle();//コンストラクタ
	~Demon_Idle();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

