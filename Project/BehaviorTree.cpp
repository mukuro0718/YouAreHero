#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BehaviorTree::BehaviorTree()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BehaviorTree::~BehaviorTree()
{

}

/// <summary>
/// �o�g���A�N�V�����̓o�^
/// </summary>
void BehaviorTree::EntryCurrentBattleAction(BehaviorTreeNode& _action)
{
	this->currentBattleNode = &_action;
	this->isSelectedBattleAction = true;
}
/// <summary>
/// ���A�N�V�����A�N�V�����̓o�^
/// </summary>
void BehaviorTree::EntryCurrentReaction(BehaviorTreeNode& _action)
{
	this->currentReactionNode = &_action;
	this->isSelectedReaction = true;
}
/// <summary>
/// ���A�N�V�����A�N�V�����̉���
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
/// �o�g���A�N�V�����̉���
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