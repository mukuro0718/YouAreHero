//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_Idle : public ActionNode
{
public:
	 Tank_Idle();//�R���X�g���N�^
	~Tank_Idle();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

