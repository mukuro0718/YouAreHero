//===========================================================
// @brief �o�g���A�N�V�������I������Ă��邩
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsSelectedBattleAction : public ConditionNode
{
public:
	 Condition_IsSelectedBattleAction();//�R���X�g���N�^
	~Condition_IsSelectedBattleAction();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//�X�V
};

