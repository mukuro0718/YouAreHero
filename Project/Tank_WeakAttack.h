//=====================================================
// @brief 弱攻撃アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_WeakAttack : public ActionNode
{
public:
	 Tank_WeakAttack();//コンストラクタ
	~Tank_WeakAttack();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//更新
	void	  Initialize()override;						//初期化
private:
	bool  isInitialize;		//初期化したか
};

