#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Beast_PlayCurrentBattleAction.h"
#include "BeastBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Beast_PlayCurrentBattleAction::Beast_PlayCurrentBattleAction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Beast_PlayCurrentBattleAction::~Beast_PlayCurrentBattleAction()
{

}

/// <summary>
/// 更新処理
/// </summary>
Beast_PlayCurrentBattleAction::NodeState Beast_PlayCurrentBattleAction::Update()
{
	//アクションの状態をセット
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	return rootNode.GetBattleAction().Update();
}
