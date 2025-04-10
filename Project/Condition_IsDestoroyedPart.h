//===========================================================
// @briefどこかの部位が破壊されていたら
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsDestoroyedPart : public ConditionNode
{
public:
	 Condition_IsDestoroyedPart();//コンストラクタ
	~Condition_IsDestoroyedPart();//デストラクタ

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//更新
};

