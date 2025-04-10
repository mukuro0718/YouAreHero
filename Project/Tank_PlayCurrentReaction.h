//=====================================================
// @brief 保存したアクションを実行
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_PlayCurrentReaction : public ActionNode
{
public:
	 Tank_PlayCurrentReaction();//コンストラクタ
	~Tank_PlayCurrentReaction();//デストラクタ

	void		Initialize	();													//初期化
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//更新
};

