//===========================================================
// @brief �O�̃A�N�V�������A�]���������A�N�V�����Ɠ�����
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsPrevActionIsSameAsSpecifiedAction : public ConditionNode
{
public:
	 Condition_IsPrevActionIsSameAsSpecifiedAction(const int _specifiedAction);//�R���X�g���N�^
	~Condition_IsPrevActionIsSameAsSpecifiedAction();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//�X�V
private:
	const int SPECIFIED_ACTION;
};

