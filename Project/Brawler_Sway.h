//=====================================================
// @brief ����A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_Sway : public ActionNode
{
public:
	 Brawler_Sway();//�R���X�g���N�^
	~Brawler_Sway();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

