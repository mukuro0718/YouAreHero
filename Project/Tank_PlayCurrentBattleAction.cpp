#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h" 
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Tank_PlayCurrentBattleAction.h"
#include "TankEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Tank_PlayCurrentBattleAction::Tank_PlayCurrentBattleAction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Tank_PlayCurrentBattleAction::~Tank_PlayCurrentBattleAction()
{

}

/// <summary>
/// 更新処理
/// </summary>
Tank_PlayCurrentBattleAction::NodeState Tank_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//アクションの状態をセット
	return _tree.GetBattleAction().Update(_tree, _chara);
}
