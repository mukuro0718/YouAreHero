//=====================================================
// @brief ���U���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_StrongAttack : public ActionNode
{
public:
	 Tank_StrongAttack();//�R���X�g���N�^
	~Tank_StrongAttack();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

