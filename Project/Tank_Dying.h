//=====================================================
// @brief �f�X�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_Dying : public ActionNode
{
public:
	 Tank_Dying();//�R���X�g���N�^
	~Tank_Dying();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

