//=======================================================
// @brief ���ݒl���萔�ȏォ
//=======================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsFrighteningValueGreaterThanConstant : public ConditionNode
{
public:
	 Condition_IsFrighteningValueGreaterThanConstant(const int _specifiedFrighteningValue);//�R���X�g���N�^
	~Condition_IsFrighteningValueGreaterThanConstant();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V
private:
	const int SPECIFIED_FRIGHTENING_VALUE;//���ݒl
};

