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
Condition_IsDragonActionIntervalIsOver::Condition_IsDragonActionIntervalIsOver(const int _actionIndex)
	: ACTION_INDEX (_actionIndex)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Condition_IsDragonActionIntervalIsOver::~Condition_IsDragonActionIntervalIsOver()
{

}

/// <summary>
/// 更新
/// </summary>
Condition_IsDragonActionIntervalIsOver::NodeState Condition_IsDragonActionIntervalIsOver::Update()
{
	auto& rootNode = Singleton<DragonBehaviorTree>::GetInstance();

	/*スタミナが０より多ければSUCCESSを返す*/
	if (rootNode.GetInterval(this->ACTION_INDEX) > 0)
	{
		//printfDx("CONDITION_STAMINA_SUCCESS\n");
		return NodeState::SUCCESS;
	}
	//printfDx("CONDITION_STAMINA_FAILURE\n");
	return NodeState::FAILURE;
}