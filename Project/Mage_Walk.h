//=====================================================
// @brief �����A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Mage_Walk : public ActionNode
{
public:
	 Mage_Walk();//�R���X�g���N�^
	~Mage_Walk();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

