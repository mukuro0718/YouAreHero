//===========================================================
// @brief �U���������邩
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsCanAttack : public ConditionNode
{
public:
	 Condition_IsCanAttack();//�R���X�g���N�^
	~Condition_IsCanAttack();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V
};

