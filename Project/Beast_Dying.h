//=====================================================
// @brief �f�X�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Beast_Dying : public ActionNode
{
public:
	 Beast_Dying();//�R���X�g���N�^
	~Beast_Dying();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara)override;//�X�V
private:
	bool  isInitialize;		//������������
};

