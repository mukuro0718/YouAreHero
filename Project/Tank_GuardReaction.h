//=====================================================
// @brief �K�[�h���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_GuardReaction : public ActionNode
{
public:
	 Tank_GuardReaction();//�R���X�g���N�^
	~Tank_GuardReaction();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

