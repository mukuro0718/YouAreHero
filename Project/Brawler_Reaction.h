//=====================================================
// @brief ���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_Reaction : public ActionNode
{
public:
	 Brawler_Reaction();//�R���X�g���N�^
	~Brawler_Reaction();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

