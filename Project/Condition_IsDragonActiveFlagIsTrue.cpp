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
Condition_IsDragonActiveFlagIsTrue::Condition_IsDragonActiveFlagIsTrue()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsDragonActiveFlagIsTrue::~Condition_IsDragonActiveFlagIsTrue()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsDragonActiveFlagIsTrue::NodeState Condition_IsDragonActiveFlagIsTrue::Update()
{
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();

	/*スタミナが０より多ければSUCCESSを返す*/
	if (rootNode.GetIsActive())
	{
		//printfDx("CONDITION_ACTIVE_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_ACTIVE_FAILURE\n");
	return NodeState::FAILURE;
}