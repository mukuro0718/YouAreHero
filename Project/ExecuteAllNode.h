//=====================================================
// @brief �G�N�X�L���[�g�I�[���m�[�h
// �q�����ׂĎ��s����B
// ���ׂĂ̎q�m�[�h�����������瑦���Ɋ�����Ԃ�
//=====================================================
#pragma once

class CompositeNode;
class ExecuteAllNode : public CompositeNode
{
public:
	 ExecuteAllNode();//�R���X�g���N�^
	~ExecuteAllNode();//�f�X�g���N�^

	NodeState Update() override;
};
