#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DragonBehaviorTreeHeader.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Condition_IsSelectedDragonBattleAction::Condition_IsSelectedDragonBattleAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsSelectedDragonBattleAction::~Condition_IsSelectedDragonBattleAction()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsSelectedDragonBattleAction::NodeState Condition_IsSelectedDragonBattleAction::Update()
{
	/*部位破壊されていたら成功*/
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();
	if (rootNode.GetIsSelectedBattleAction())
	{
		//printfDx("CONDITION_SELECTED_BATTLE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_SELECTED_BATTLE_FAILURE\n");
	return NodeState::FAILURE;
}