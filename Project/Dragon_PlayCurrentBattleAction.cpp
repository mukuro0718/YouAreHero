#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h"
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
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
Dragon_PlayCurrentBattleAction::NodeState Dragon_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//アクションの状態をセット
	return _tree.GetBattleAction().Update(_tree, _chara);
}
