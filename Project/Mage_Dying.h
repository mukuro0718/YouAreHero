//=====================================================
// @brief �f�X�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Dying : public ActionNode
{
public:
	 Mage_Dying();//�R���X�g���N�^
	~Mage_Dying();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

