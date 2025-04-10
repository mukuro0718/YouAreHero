#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "CompositeNode.h"
#include "ExecuteAllNode.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ExecuteAllNode::ExecuteAllNode()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ExecuteAllNode::~ExecuteAllNode()
{

}

/// <summary>
/// �X�V
/// </summary>
ExecuteAllNode::NodeState ExecuteAllNode::Update(BehaviorTree& _tree, Character& _chara)
{
	int successCount = 0;
	/*���ׂĂ̎q�����s����*/
	for (auto& child : this->children)
	{
		//�������Ă�����J�E���g�𑝂₷
		if (child->Update(_tree, _chara) == NodeState::SUCCESS)
		{
			successCount++;
		}
	}

	/*�q�����ׂĐ��������犮����Ԃ�*/
	if (successCount == this->children.size())
	{
		return NodeState::COMPLETED;
	}
	return NodeState::RUNNING;
}