//===========================================================
// @brief �h���S���̏�Ԃ�]��
//===========================================================
#pragma once

class ConditionNode;
class Condition_IsDragonStateIsSameAsTheSpecifiedState : public ConditionNode
{
public:
	 Condition_IsDragonStateIsSameAsTheSpecifiedState(const int _specifiedState);//�R���X�g���N�^
	~Condition_IsDragonStateIsSameAsTheSpecifiedState();//�f�X�g���N�^

	NodeState Update	() override;//�X�V
private:
	const int SPECIFIED_STATE;

};

