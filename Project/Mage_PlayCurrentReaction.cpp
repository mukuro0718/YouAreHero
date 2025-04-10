#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Mage_PlayCurrentReaction.h"
#include "MageEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Mage_PlayCurrentReaction::Mage_PlayCurrentReaction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Mage_PlayCurrentReaction::~Mage_PlayCurrentReaction()
{

}


/// <summary>
/// 初期化
/// </summary>
void Mage_PlayCurrentReaction::Initialize()
{
}

/// <summary>
/// 更新処理
/// </summary>
Mage_PlayCurrentReaction::NodeState Mage_PlayCurrentReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	return _tree.GetReaction().Update(_tree, _chara);
}
