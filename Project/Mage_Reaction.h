//=====================================================
// @brief ���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Reaction : public ActionNode
{
public:
	 Mage_Reaction();//�R���X�g���N�^
	~Mage_Reaction();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

