#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"
#include "SelectorNode.h"

SelectorNode::SelectorNode()
{

}


SelectorNode::~SelectorNode()
{

}

/// <summary>
/// çXêV
/// </summary>
SelectorNode::NodeState SelectorNode::Update()
{
	for (auto& child : this->children)
	{
		if (child->Update() == NodeState::SUCCESS)
		{
			return NodeState::SUCCESS;
		}
	}
	return NodeState::FAILURE;
}