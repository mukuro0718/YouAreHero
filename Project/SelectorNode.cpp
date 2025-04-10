#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "CompositeNode.h"
#include "SelectorNode.h"

SelectorNode::SelectorNode()
{

}


SelectorNode::~SelectorNode()
{

}

/// <summary>
/// �X�V
/// </summary>
SelectorNode::NodeState SelectorNode::Update(BehaviorTree& _tree, Character& _chara)
{
	/*�q�����s*/
	for (auto& child : this->children)
	{
		NodeState state = child->Update(_tree, _chara);
		//��Ԃ������Ȃ琬����Ԃ��I��
		if (state == NodeState::SUCCESS)
		{
			//printfDx("SELECTOR_SUCCESS\n");
			return NodeState::SUCCESS;
		}
		//��Ԃ����s���Ȃ���s����Ԃ��I��
		if (state == NodeState::RUNNING)
		{
			//printfDx("SELECTOR_RUNNING\n");
			return NodeState::RUNNING;
		}
		//printfDx("SELECTOR_FAILURE\n");
	}
	return NodeState::FAILURE;
}