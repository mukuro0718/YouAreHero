//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Demon_Idle : public ActionNode
{
public:
	 Demon_Idle();//�R���X�g���N�^
	~Demon_Idle();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

