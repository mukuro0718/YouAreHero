//=====================================================
// @brief ��U���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_WeakAttack : public ActionNode
{
public:
	 Tank_WeakAttack();//�R���X�g���N�^
	~Tank_WeakAttack();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

