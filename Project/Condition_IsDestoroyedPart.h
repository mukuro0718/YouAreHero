//===========================================================
// @briefどこかの部位が破壊されていたら
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDestoroyedPart : public ConditionNode
{
public:
	 Condition_IsDestoroyedPart();//コンストラクタ
	~Condition_IsDestoroyedPart();//デストラクタ

	NodeState Update	() override;//更新
};

