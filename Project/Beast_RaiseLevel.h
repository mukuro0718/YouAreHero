//=====================================================
// @brief ���x���㏸�����K�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_RaiseLevel : public ActionNode
{
public:
	 Beast_RaiseLevel();//�R���X�g���N�^
	~Beast_RaiseLevel();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	short effectStartCount;	//�G�t�F�N�g�Đ��J�n�t���[��
	short frameCount;		//�t���[���J�E���g
	short roarFrame;	//���K�̃t���[��
};

