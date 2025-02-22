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
Conditon_IsAttackCountIsOver::Conditon_IsAttackCountIsOver()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Conditon_IsAttackCountIsOver::~Conditon_IsAttackCountIsOver()
{

}

/// <summary>
/// 更新
/// </summary>
Conditon_IsAttackCountIsOver::NodeState Conditon_IsAttackCountIsOver::Update()
{
	auto& boss = Singleton<EnemyManager>::GetInstance();

	/*インターバルが終了していなければFAILUREを返す*/
	if (boss.GetAttackCount() <= 0)
	{
		//printfDx("CONDITION_INTERVAL_FAILURE\n");
		return NodeState::FAILURE;
	}
	//printfDx("CONDITION_INTERVAL_SUCCESS\n");
	return NodeState::SUCCESS;
}