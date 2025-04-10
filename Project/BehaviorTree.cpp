#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
BehaviorTree::BehaviorTree()
{

}

/// <summary>
/// デストラクタ
/// </summary>
BehaviorTree::~BehaviorTree()
{

}

/// <summary>
/// バトルアクションの登録
/// </summary>
void BehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleNode = &_action;
	this->isSelectedBattleAction = true;
}
/// <summary>
/// リアクションアクションの登録
/// </summary>
void BehaviorTree::EntryCurrentReaction(BehaviorTreeNode& _action)
{
	this->currentReactionNode = &_action;
	this->isSelectedReaction = true;
}
/// <summary>
/// リアクションアクションの解除
/// </summary>
void BehaviorTree::ExitCurrentReaction()
{
	this->isSelectedReaction = false;
	if (this->currentReactionNode != nullptr)
	{
		this->currentReactionNode->Initialize();
	}
	this->currentReactionNode = nullptr;
}
/// <summary>
/// バトルアクションの解除
/// </summary>
void BehaviorTree::ExitCurrentBattleAction()
{
	this->isSelectedBattleAction = false;
	if (this->currentBattleNode != nullptr)
	{
		this->currentBattleNode->Initialize();
	}
	this->currentBattleNode = nullptr;
}