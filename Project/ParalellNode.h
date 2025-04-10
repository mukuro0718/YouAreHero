//=====================================================
// @brief �p�������m�[�h
// �q�����Ԃɕ]�����A���ׂĂ̎q�m�[�h����������܂Ŏ��s����B
// �����ꂩ�̎q�m�[�h�����s�����瑦���Ɏ��s��Ԃ�
//=====================================================
#pragma once

class BehaviorTree;
class CompositeNode;
class Character;
class ParalellNode : public CompositeNode
{
public:
	 ParalellNode();//�R���X�g���N�^
	~ParalellNode();//�f�X�g���N�^

	NodeState Update(BehaviorTree& _tree, Character& _chara) override;
protected:
	/*�����o�ϐ�*/
	int successThreshold;//�����������l
	int failureThreshold;//���s�������l
	int successCount;//����������
	int failureCount;//���s������
};

