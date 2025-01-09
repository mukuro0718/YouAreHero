#include <DxLib.h>
#include "UseSTL.h"
#include "BehaviorTreeNode.h"
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
/// 更新処理
/// </summary>
Beast_PlayCurrentReaction::NodeState Beast_PlayCurrentReaction::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	return rootNode.GetReaction().Update();
}
