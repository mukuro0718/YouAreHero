//=====================================================
// @brief 左へ回るアクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_TurnLeft : public ActionNode
{
public:
	 Beast_TurnLeft();//コンストラクタ
	~Beast_TurnLeft();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
private:
	short maxFrameCount;
	short frameCount;
};

