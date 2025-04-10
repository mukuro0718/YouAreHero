//=====================================================
// @brief 保存したアクションを実行
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_PlayCurrentReaction : public ActionNode
{
public:
	 Mage_PlayCurrentReaction();//コンストラクタ
	~Mage_PlayCurrentReaction();//デストラクタ

	void		Initialize	();													//初期化
	NodeState	Update		(BehaviorTree& _tree, Character& _chara)override;	//更新
};

