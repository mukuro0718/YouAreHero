//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Beast_Idle : public ActionNode
{
public:
	 Beast_Idle();//�R���X�g���N�^
	~Beast_Idle();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

