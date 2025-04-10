//=======================================================
// @brief �{��l���m�F����R���f�B�V�����m�[�h
//=======================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsAngryValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsAngryValueGreaterThanConstant(const int _specifiedAngryValue);//�R���X�g���N�^
	~Condition_IsAngryValueGreaterThanConstant();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V

private:
	const int SPECIFIED_ANGRY_VALUE;//�{��l

};

