#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "CompositeNode.h"
#include "SequencerNode.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SequencerNode::SequencerNode()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SequencerNode::~SequencerNode()
{

}

/// <summary>
/// �X�V
/// </summary>
SequencerNode::NodeState SequencerNode::Update(BehaviorTree& _tree, Character& _chara)
{

	/*�q�����s*/
	for (auto& child : this->children)
	{
		NodeState state = child->Update(_tree, _chara);
		//���s���Ă������Ԃ�Ԃ�
		if (state == NodeState::FAILURE)
		{
			//printfDx("SEQUENCER_FAILURE\n");
			return NodeState::FAILURE;
		}
		//���s���Ȃ��Ԃ�Ԃ�
		if (state == NodeState::RUNNING)
		{
			//printfDx("SEQUENCER_RUNNING\n");
			return NodeState::RUNNING;
		}
		//printfDx("SEQUENCER_SUCCESS\n");
	}
	return NodeState::SUCCESS;
}