//=====================================================
// @brief ���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_Reaction : public ActionNode
{
public:
	 Tank_Reaction();//�R���X�g���N�^
	~Tank_Reaction();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

