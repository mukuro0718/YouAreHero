#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
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
Condition_IsSelectedBattleAction::NodeState Condition_IsSelectedBattleAction::Update()
{
	/*部位破壊されていたら成功*/
	auto& rootNode = Singleton<BeastBehaviorTree>::GetInstance();
	if (rootNode.GetIsSelectedBattleAction())
	{
		//printfDx("CONDITION_SELECTED_BATTLE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_BATTLE_FAILURE\n");
	return NodeState::FAILURE;
}