//=====================================================
// @brief �R���f�B�V�����m�[�h
// ������]�����A�����܂��͎��s��Ԃ��B
// ��������������Ă��邩�ǂ������`�F�b�N����
// �p����ɏڍׂȏ���������
//=====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class ConditionNode abstract: public BehaviorTreeNode
{
public:
			 ConditionNode(){}//�R���X�g���N�^
	virtual ~ConditionNode(){}//�f�X�g���N�^

	virtual NodeState Update(BehaviorTree& _tree, Character& _chara) abstract;//�X�V
};

