//===========================================================
// @brief�ǂ����̕��ʂ��j�󂳂�Ă�����
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsDestoroyedPart : public ConditionNode
{
public:
	 Condition_IsDestoroyedPart();//�R���X�g���N�^
	~Condition_IsDestoroyedPart();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//�X�V
};

