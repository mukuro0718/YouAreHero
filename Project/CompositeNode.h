//=====================================================
// @brief �R���|�W�b�g�m�[�h
// �����̎q�m�[�h���O���[�v�����A�����̓�����Ǘ�����
//=====================================================
#pragma once

class BehaviorTree;
class BehaviorTreeNode;
class Character;
class CompositeNode abstract : public BehaviorTreeNode
{
public:
			 CompositeNode();//�R���X�g���N�^
	virtual ~CompositeNode();//�f�X�g���N�^

	virtual NodeState Update  (BehaviorTree& _tree, Character& _chara)  abstract;//�X�V
			void	  AddChild(BehaviorTreeNode& _child)				override;//�q���̒ǉ�
protected:
	std::vector<BehaviorTreeNode*> children;
};

