#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "BeastBehaviorTreeHeader.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsSelectedBattleAction::Condition_IsSelectedBattleAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsSelectedBattleAction::~Condition_IsSelectedBattleAction()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsSelectedBattleAction::NodeState Condition_IsSelectedBattleAction::Update(BehaviorTree& _tree, Character& _chara)
{
	/*攻撃アクションが選択されていたら成功*/
	if (_tree.GetIsSelectedBattleAction())
	{
		//printfDx("CONDITION_SELECTED_BATTLE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_BATTLE_FAILURE\n");
	return NodeState::FAILURE;
}