#include "BehaviorTreeNode.h"
#include "DecoratorNode.h"

/// <summary>
/// コンストラクタ
/// </summary>
DecoratorNode::DecoratorNode()
	: child(nullptr)
{

}

/// <summary>
/// デストラクタ
/// </summary>
DecoratorNode::~DecoratorNode()
{

}

/// <summary>
/// 子供の追加
/// </summary>
void DecoratorNode::AddChild(BehaviorTreeNode* _child)
{
	this->child = _child;
}