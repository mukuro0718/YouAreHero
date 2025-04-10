#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Character.h" 
#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"
#include "ActionNode.h"
#include "Brawler_PlayCurrentBattleAction.h"
#include "BrawlerEnemyBehaviorTree.h"

/// <summary>
/// コンストラクタ
/// </summary>
Brawler_PlayCurrentBattleAction::Brawler_PlayCurrentBattleAction()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Brawler_PlayCurrentBattleAction::~Brawler_PlayCurrentBattleAction()
{

}

/// <summary>
/// 更新処理
/// </summary>
Brawler_PlayCurrentBattleAction::NodeState Brawler_PlayCurrentBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	//アクションの状態をセット
	return _tree.GetBattleAction().Update(_tree, _chara);
}
