//=====================================================
// @brief �_�b�V���A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_Run : public ActionNode
{
public:
	 Tank_Run();//�R���X�g���N�^
	~Tank_Run();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

