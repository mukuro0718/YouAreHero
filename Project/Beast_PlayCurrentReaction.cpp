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
/// 初期化
/// </summary>
void Beast_PlayCurrentReaction::Initialize()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	rootNode.Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
Beast_PlayCurrentReaction::NodeState Beast_PlayCurrentReaction::Update()
{
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	return rootNode.GetReaction().Update();
}
