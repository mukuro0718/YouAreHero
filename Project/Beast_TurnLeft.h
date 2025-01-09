//=====================================================
// @brief 左へ回るアクション
//=====================================================
#pragma once

class ActionNode;
class Beast_TurnLeft : public ActionNode
{
public:
	 Beast_TurnLeft();//コンストラクタ
	~Beast_TurnLeft();//デストラクタ

	NodeState Update()override;//更新
private:
	short maxFrameCount;
	short frameCount;
};

