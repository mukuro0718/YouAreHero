#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_PlayCurrentReaction.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Brawler_PlayCurrentReaction::Brawler_PlayCurrentReaction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Brawler_PlayCurrentReaction::~Brawler_PlayCurrentReaction()
{

}


/// <summary>
/// 初期化
/// </summary>
void Brawler_PlayCurrentReaction::Initialize()
{
}

/// <summary>
/// 更新処理
/// </summary>
Brawler_PlayCurrentReaction::NodeState Brawler_PlayCurrentReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	return _tree.GetReaction().Update(_tree, _chara);
}
