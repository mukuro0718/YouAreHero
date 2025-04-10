//===========================================================
// @brief HP���萔�ȉ���������
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Condition_IsHpBelowConstant : public ConditionNode
{
public:
	 Condition_IsHpBelowConstant(const int _specifiedHp);//�R���X�g���N�^
	~Condition_IsHpBelowConstant();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara) override;//�X�V
private:
	const int SPECIFIED_HP;//HP
};

