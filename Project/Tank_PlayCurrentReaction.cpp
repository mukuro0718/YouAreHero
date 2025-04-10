#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_PlayCurrentReaction.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Tank_PlayCurrentReaction::Tank_PlayCurrentReaction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Tank_PlayCurrentReaction::~Tank_PlayCurrentReaction()
{

}


/// <summary>
/// 初期化
/// </summary>
void Tank_PlayCurrentReaction::Initialize()
{
}

/// <summary>
/// 更新処理
/// </summary>
Tank_PlayCurrentReaction::NodeState Tank_PlayCurrentReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	return _tree.GetReaction().Update(_tree, _chara);
}
