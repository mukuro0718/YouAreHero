//=====================================================
// @brief �f�X�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_Dying : public ActionNode
{
public:
	 Brawler_Dying();//�R���X�g���N�^
	~Brawler_Dying();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

