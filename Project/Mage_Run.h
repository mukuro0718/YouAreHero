//=====================================================
// @brief �_�b�V���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Run : public ActionNode
{
public:
	 Mage_Run();//�R���X�g���N�^
	~Mage_Run();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

