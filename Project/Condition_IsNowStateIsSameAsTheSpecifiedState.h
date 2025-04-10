//===========================================================
// @brief �{�X�̏�Ԃ��]����������ԂƓ�����
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsNowStateIsSameAsTheSpecifiedState : public ConditionNode
{
public:
	 Condition_IsNowStateIsSameAsTheSpecifiedState(const int _specifiedState);//�R���X�g���N�^
	~Condition_IsNowStateIsSameAsTheSpecifiedState();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V
private:
	const int SPECIFIED_STATE;
};

