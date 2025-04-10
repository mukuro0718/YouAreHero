#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h" 
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
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
Beast_PlayCurrentBattleAction::NodeState Beast_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//アクションの状態をセット
	return _tree.GetBattleAction().Update(_tree, _chara);
}
