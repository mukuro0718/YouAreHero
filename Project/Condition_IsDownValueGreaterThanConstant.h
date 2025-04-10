//=======================================================
// @brief �_�E�����m�F����R���f�B�V�����m�[�h
//=======================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsDownValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsDownValueGreaterThanConstant(const int _specifiedDownValue);//�R���X�g���N�^
	~Condition_IsDownValueGreaterThanConstant();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V

private:
	const int SPECIFIED_DOWN_VALUE;//�_�E���l

};

