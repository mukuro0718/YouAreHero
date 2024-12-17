//=====================================================
// @brief 右に回るアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_TurnRight : public ActionNode
{
public:
	 Beast_TurnRight();//コンストラクタ
	~Beast_TurnRight();//デストラクタ

	NodeState Update()override;//更新
};

