//===========================================================
// @brief ���A�N�V�������I������Ă��邩
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsSelectedReaction : public ConditionNode
{
public:
	 Condition_IsSelectedReaction();//�R���X�g���N�^
	~Condition_IsSelectedReaction();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//�X�V
};

