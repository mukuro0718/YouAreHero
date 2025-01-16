#include <DxLib.h>
#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "RandomSelector.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
RandomSelector::RandomSelector()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
RandomSelector::~RandomSelector()
{

}

/// <summary>
/// �X�V
/// </summary>
BehaviorTreeNode::NodeState RandomSelector::Update()
{
	/*���s����q���̔ԍ����擾*/
	int random = GetRand(this->children.size() - 1);

	/*�q�����s*/
	NodeState state = this->children[random]->Update();
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
	return NodeState::FAILURE;

}