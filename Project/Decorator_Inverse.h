//=====================================================
// @brief �����_���Z���N�^�[�m�[�h
// �q�m�[�h�������_���őI������
//=====================================================
#pragma once

class DecoratorNode;
class Decorator_Inverse : public DecoratorNode
{
public:
			 Decorator_Inverse();//�R���X�g���N�^
	virtual ~Decorator_Inverse();//�f�X�g���N�^

	NodeState Update() override;//�X�V
};

