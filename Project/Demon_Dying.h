//=====================================================
// @brief �f�X�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Demon_Dying : public ActionNode
{
public:
	 Demon_Dying();//�R���X�g���N�^
	~Demon_Dying();//�f�X�g���N�^

	NodeState Update	()override;//�X�V
	void	  Initialize()override;//������
private:
	bool  isInitialize;		//������������
};

