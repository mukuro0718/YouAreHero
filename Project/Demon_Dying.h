//=====================================================
// @brief �f�X�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Demon_Dying : public ActionNode
{
public:
	 Demon_Dying();//�R���X�g���N�^
	~Demon_Dying();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

