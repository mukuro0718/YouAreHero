//=====================================================
// @brief �����A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_Walk : public ActionNode
{
public:
	 Tank_Walk();//�R���X�g���N�^
	~Tank_Walk();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

