//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_Idle : public ActionNode
{
public:
	 Brawler_Idle();//�R���X�g���N�^
	~Brawler_Idle();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

