#include "BehaviorTreeNode.h"
#include "DecoratorNode.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
DecoratorNode::DecoratorNode()
	: child(nullptr)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
DecoratorNode::~DecoratorNode()
{

}

/// <summary>
/// �q���̒ǉ�
/// </summary>
void DecoratorNode::AddChild(BehaviorTreeNode& _child)
{
	this->child = &_child;
}