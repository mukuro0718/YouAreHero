//=====================================================
// @brief �X�e�b�v�o�b�N�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_StepBack : public ActionNode
{
public:
	 Beast_StepBack();//�R���X�g���N�^
	~Beast_StepBack();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

