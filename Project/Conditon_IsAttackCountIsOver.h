//===========================================================
// @brief �U���񐔂��c���Ă��邩�𔻒肷��R���f�B�V�����m�[�h
//===========================================================
#pragma once

class BehaviorTree;
class ConditionNode;
class Character;
class Conditon_IsAttackCountIsOver : public ConditionNode
{
public:
	 Conditon_IsAttackCountIsOver();//�R���X�g���N�^
	~Conditon_IsAttackCountIsOver();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;//�X�V
};

