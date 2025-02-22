#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "DeleteInstance.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CompositeNode::CompositeNode()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CompositeNode::~CompositeNode()
{
	for (int i = 0; i < this->children.size(); i++)
	{
		DeleteMemberInstance(this->children[i]);
	}
	this->children.clear();
}

/// <summary>
/// �q���̒ǉ�
/// </summary>
void CompositeNode::AddChild(BehaviorTreeNode& _child)
{
	this->children.emplace_back(&_child);
}