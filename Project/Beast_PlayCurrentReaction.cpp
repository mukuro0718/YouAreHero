#include <DxLib.h>
#include "UseSTL.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Beast_PlayCurrentReaction.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_PlayCurrentReaction::Beast_PlayCurrentReaction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Beast_PlayCurrentReaction::~Beast_PlayCurrentReaction()
{

}


/// <summary>
/// 初期化
/// </summary>
void Beast_PlayCurrentReaction::Initialize()
{
}

/// <summary>
/// 更新処理
/// </summary>
Beast_PlayCurrentReaction::NodeState Beast_PlayCurrentReaction::Update(BehaviorTree& _tree, Character& _chara)
{
	return _tree.GetReaction().Update(_tree, _chara);
}
