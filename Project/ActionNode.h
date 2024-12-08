//===========================================
// @brief アクションノード
// ゲームキャラクターの実際のふるまいを書く
//===========================================
#pragma once

class BehaviorTreeNode;
class ActionNode abstract : public BehaviorTreeNode
{
public:
	/*列挙体*/


	ActionNode(){}//コンストラクタ
	virtual ~ActionNode(){}//デストラクタ

	virtual NodeState Update() abstract;//更新
};

