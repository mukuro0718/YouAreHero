//=====================================================
// @brief �f�R���[�^�[�m�[�h
// �q�̂ӂ�܂���ύX����m�[�h
// ��ɃA�N�V�����̎��s�̑O��Œǉ��̏������s������A
// �]�����ʂ�ύX�����肷��
//=====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class DecoratorNode abstract: public BehaviorTreeNode
{
public:
			 DecoratorNode();//�R���X�g���N�^
	virtual ~DecoratorNode();//�f�X�g���N�^

	virtual NodeState Update	(BehaviorTree& _tree, Character& _chara)abstract;//�X�V
			void	  AddChild	(BehaviorTreeNode& _child)				override;//�q���̒ǉ�
protected:
	BehaviorTreeNode* child;
};

