//===========================================================
// @brief �U�����󂯂Ă��邩
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsHitAttack : public ConditionNode
{
public:
	 Condition_IsHitAttack();//�R���X�g���N�^
	~Condition_IsHitAttack();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//�X�V
};

