//=====================================================
// @brief �ҋ@�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Demon_Idle : public ActionNode
{
public:
	 Demon_Idle();//�R���X�g���N�^
	~Demon_Idle();//�f�X�g���N�^

	NodeState Update	()override;//�X�V
	void	  Initialize()override;//������
private:
	bool  isInitialize;		//������������
};

