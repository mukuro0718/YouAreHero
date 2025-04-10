//=====================================================
// @brief 待機アクション
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Walk : public ActionNode
{
public:
	 Beast_Walk();//コンストラクタ
	~Beast_Walk();//デストラクタ

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//更新
private:
	short frameCount;		//フレームカウント
	short walkSoundCount;	//歩きの音を出すカウント
};

