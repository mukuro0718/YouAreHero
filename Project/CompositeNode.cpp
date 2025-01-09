#include "UseSTL.h"
#include "BehaviorTreeNode.h"
#include "CompositeNode.h"

/// <summary>
/// コンストラクタ
/// </summary>
CompositeNode::CompositeNode()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CompositeNode::~CompositeNode()
{
	this->children.clear();
}

/// <summary>
/// 子供の追加
/// </summary>
void CompositeNode::AddChild(BehaviorTreeNode& _child)
{
	this->children.emplace_back(&_child);
}