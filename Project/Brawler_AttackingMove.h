//=====================================================
// @brief �_�b�V���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Brawler_AttackingMove : public ActionNode
{
public:
	 Brawler_AttackingMove();//�R���X�g���N�^
	~Brawler_AttackingMove();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

