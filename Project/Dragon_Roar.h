//=====================================================
// @brief ���K�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Dragon_Roar : public ActionNode
{
public:
	 Dragon_Roar();//�R���X�g���N�^
	~Dragon_Roar();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	short frameCount;	//�t���[���J�E���g
	short roarFrame;	//���K�̃t���[��
};

