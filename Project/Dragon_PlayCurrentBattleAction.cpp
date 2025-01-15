#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BehaviorTreeNode.h"
#include "ActionNode.h"
#include "Dragon_PlayCurrentBattleAction.h"
#include "DragonBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Dragon_PlayCurrentBattleAction::Dragon_PlayCurrentBattleAction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Dragon_PlayCurrentBattleAction::~Dragon_PlayCurrentBattleAction()
{

}

/// <summary>
/// 更新処理
/// </summary>
Dragon_PlayCurrentBattleAction::NodeState Dragon_PlayCurrentBattleAction::Update()
{
	//アクションの状態をセット
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	return rootNode.GetBattleAction().Update();
}
