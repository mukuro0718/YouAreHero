//=====================================================
// @brief �_�b�V���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Run : public ActionNode
{
public:
	 Beast_Run();//�R���X�g���N�^
	~Beast_Run();//�f�X�g���N�^

	NodeState	Update(BehaviorTree& _tree, Character& _chara)override;	//�X�V
};

