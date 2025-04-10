//=====================================================
// @brief 強攻撃アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_StrongAttack : public ActionNode
{
public:
	 Tank_StrongAttack();//コンストラクタ
	~Tank_StrongAttack();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

