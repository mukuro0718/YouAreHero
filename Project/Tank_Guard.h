//=====================================================
// @brief �K�[�h�A�N�V����
//=====================================================
#pragma once

class BehaviorTree;
class ActionNode;
class Character;
class Tank_Guard : public ActionNode
{
public:
	 Tank_Guard();//�R���X�g���N�^
	~Tank_Guard();//�f�X�g���N�^

	NodeState Update	(BehaviorTree& _tree, Character& _chara)override;	//�X�V
	void	  Initialize()override;						//������
private:
	bool  isInitialize;		//������������
};

