#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h" 
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Mage_PlayCurrentBattleAction.h"
#include "MageEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Mage_PlayCurrentBattleAction::Mage_PlayCurrentBattleAction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Mage_PlayCurrentBattleAction::~Mage_PlayCurrentBattleAction()
{

}

/// <summary>
/// 更新処理
/// </summary>
Mage_PlayCurrentBattleAction::NodeState Mage_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//アクションの状態をセット
	return _tree.GetBattleAction().Update(_tree, _chara);
}
