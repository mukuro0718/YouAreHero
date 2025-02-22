#include <DxLib.h>
#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "DecoratorNode.h"
#include "Decorator_Inverse.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Decorator_Inverse::Decorator_Inverse()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Decorator_Inverse::~Decorator_Inverse()
{

}

/// <summary>
/// �X�V
/// </summary>
BehaviorTreeNode::NodeState Decorator_Inverse::Update()
{
	/*�q�����s*/
	NodeState state = this->child->Update();
	//��Ԃ������Ȃ琬����Ԃ��I��
	if (state == NodeState::SUCCESS)
	{
		//printfDx("SELECTOR_SUCCESS\n");
		return NodeState::FAILURE;
	}
	//��Ԃ����s���Ȃ���s����Ԃ��I��
	if (state == NodeState::FAILURE)
	{
		//printfDx("SELECTOR_RUNNING\n");
		return NodeState::SUCCESS;
	}
	//printfDx("SELECTOR_FAILURE\n");
	return NodeState::FAILURE;

}