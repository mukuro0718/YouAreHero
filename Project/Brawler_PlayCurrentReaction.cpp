#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_PlayCurrentReaction.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Brawler_PlayCurrentReaction::Brawler_PlayCurrentReaction()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Brawler_PlayCurrentReaction::~Brawler_PlayCurrentReaction()
{

}


/// <summary>
/// ������
/// </summary>
void Brawler_PlayCurrentReaction::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
Brawler_PlayCurrentReaction::NodeState Brawler_PlayCurrentReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	return _tree.GetReaction().Update(_tree, _chara);
}
